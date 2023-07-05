#ifndef __EXCHANGE_HPP__
#define __EXCHANGE_HPP__


#include <unordered_map>
#include <vector>
#include "orderBook.hpp"
#include "displayHandler.hpp"

template <typename Handler>
class Exchange {

    public:
        //constexpr static uint16_t DEFAULT_BOOKID = std::numeric_limits<uint16_t>::max();        
        Exchange(Handler& handler) : _handler{handler} {}
        
        void addOrder(const uint32_t seqno, const std::string_view symbol, const uint64_t orderId, const char side, const uint64_t qty, const int32_t price)
        {
            size_t hashCode = std::hash<std::string_view>{}(symbol);
            //Check if symbol(the hashed code) has an orderbook. If not, create one
            auto itr = _hashcodeToOrderbookIdMap.find(hashCode);
            if(itr == _hashcodeToOrderbookIdMap.end())
            {
                _orderBooks.emplace_back(OrderBook());
                itr = _hashcodeToOrderbookIdMap.emplace(hashCode, _orderBooks.size() - 1).first;
            }

            //insert the order to the map
            Side bs = 'B' == side ? Side::Buy : Side::Sell;
            _orderIdToOrderMap.emplace(orderId, Order{price, qty, bs ,itr->second});

            //Update the orderBook
            OrderBook& ob = _orderBooks[itr->second];            
            uint64_t level = ob.add(seqno, bs, qty, price);
            _handler.onQuote(seqno, symbol, ob, level);        
        }

        void updateOrder(const uint32_t seqno, const std::string_view symbol, const uint64_t orderId, const uint64_t qty, const int32_t price)
        {
            //find the order
            auto itr = _orderIdToOrderMap.find(orderId);
            if(_orderIdToOrderMap.end() == itr)
                return;
            
            Order& o = itr->second;

            //Somehow the order is not map to a book id
            if(DEFAULT_BOOKID == o.bookid)
                return;
            
            OrderBook& ob = _orderBooks[o.bookid];
            ob.reduce(seqno, o.buy_sell, o.qty, o.price);
            uint64_t level = ob.add(seqno, o.buy_sell, qty, price);

            //update the order lastly
            o.price = price;
            o.qty = qty;
            if(o.qty <= 0)
                _orderIdToOrderMap.erase(itr);

            _handler.onQuote(seqno, symbol, ob, level);
        }
        
        void deleteOrder(const uint32_t seqno, const std::string_view symbol, const uint64_t orderId)
        {
            //find the order
            auto itr = _orderIdToOrderMap.find(orderId);
            if(_orderIdToOrderMap.end() == itr)
                return;

            Order& o = itr->second;
            //Somehow the order is not map to a book id
            if(DEFAULT_BOOKID == o.bookid)
                return;
            
            OrderBook& ob = _orderBooks[o.bookid];
            uint64_t level = ob.reduce(seqno, o.buy_sell, o.qty, o.price);

            //remove the order from the map
            _orderIdToOrderMap.erase(itr);
            _handler.onQuote(seqno, symbol, ob, level);
        }

        void executeOrder(const uint32_t seqno, const std::string_view symbol, const uint64_t orderId, const uint64_t tradedQuantity)
        {
            //find the order
            auto itr = _orderIdToOrderMap.find(orderId);
            if(_orderIdToOrderMap.end() == itr)
                return;

            Order& o = itr->second;
            //Somehow the order is not map to a book id
            if(DEFAULT_BOOKID == o.bookid)
                return;
            
            OrderBook& ob = _orderBooks[o.bookid];
            uint64_t level = ob.reduce(seqno, o.buy_sell, tradedQuantity, o.price);

            //update the order
            o.qty -= std::min(tradedQuantity, o.qty);
            if(0 == o.qty)
                _orderIdToOrderMap.erase(itr);

            _handler.onTrade(seqno, symbol, ob, level);
        }

    private:
        std::unordered_map<std::size_t, uint16_t> _hashcodeToOrderbookIdMap;
        std::vector<OrderBook> _orderBooks;
        std::unordered_map<uint64_t, Order> _orderIdToOrderMap;
        Handler& _handler;
};

#endif