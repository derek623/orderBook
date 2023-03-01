#ifndef ORDERBOOK
#define ORDERBOOK

#include <iostream>
#include <boost/container/flat_map.hpp>

class OrderBook 
{
    public:
        struct Level {
            uint64_t aggregatedQty = 0;
            int32_t price = 0;
            uint32_t lastUpdateSeqNo = 0;
            bool buy_or_sell;

            Level(bool bs) : buy_or_sell{bs} {}
            Level(const uint64_t q, const int32_t p) : aggregatedQty{q}, price{p}{}

            friend std::ostream &operator<<(std::ostream &out, const Level &level) {
                int32_t newPrice = level.buy_or_sell ? level.price : -level.price;
                out << "(" << newPrice << ", " << level.aggregatedQty << ")";
                return out;
            }
        };
        OrderBook(){}
        constexpr static uint64_t INVALID_LEVEL = std::numeric_limits<uint64_t>::max();

        uint64_t add(const uint32_t& seqno, bool side, const uint64_t& qty, const int32_t& price)
        {
            auto& book = side ? _buy : _sell;
            int32_t newPrice = side ? price : -price; //flip the price for sell order

            auto itr = book.emplace(newPrice, Level{side}).first;
            
            Level& l = itr->second;
            l.price = newPrice;
            l.aggregatedQty += qty;   
            l.lastUpdateSeqNo = seqno;
            
            return distance(book.begin(), itr);
        }

        uint64_t reduce(const uint32_t& seqno, bool side, const uint64_t& qty, const int32_t& price)
        {
            auto& book = side ? _buy : _sell;
            int32_t newPrice = side ? price : -price; //flip the price for sell order

            auto itr = book.find(newPrice);
            if(book.end() == itr)
                return INVALID_LEVEL; // flat_map max entry is smaller than max of uint64_t

            Level& l = itr->second;
            if(qty <= l.aggregatedQty)
                l.aggregatedQty -= qty;
            else
                l.aggregatedQty = 0;

            l.lastUpdateSeqNo = seqno;
            
            if(l.aggregatedQty <= 0)
                itr = book.erase(itr);

            return distance(book.begin(), itr);;
        }

        friend std::ostream &operator<<(std::ostream &out, const OrderBook &book)
        {
            std::cout << " Buy:" << std::endl;
            for(auto b : book._buy)
                std::cout << "\t" << b.second << std::endl;
            std::cout << " Sell:" << std::endl;
            for(auto s : book._sell)
                std::cout << "\t" << s.second << std::endl;
            return out;
        }

        void print(const uint64_t level) const
        {
            int count = 1;
            std::cout << "[";
            auto bitr = _buy.begin();
            if(_buy.end() != bitr)
            {
                std::cout << bitr->second;

                for(++bitr; bitr != _buy.end() && count < level; ++bitr,++count )
                    std::cout << ", " << bitr->second;
                std::cout << "], [";
            }
            count = 1;
            auto sitr = _sell.begin();
            if(_sell.end() != sitr)
            {
                std::cout << sitr->second;
                for(++sitr; sitr != _sell.end() && count < level; ++sitr,++count )
                    std::cout << ", " << sitr->second;
            }
            std::cout << "]" << std::endl;
        }

#ifdef DEBUG_BUILD
        //For testing
        boost::container::flat_map<int64_t, Level, std::greater<int64_t>>& getBuy()
        {
            return _buy;
        }

        boost::container::flat_map<int64_t, Level, std::greater<int64_t>>& getSell()
        {
            return _sell;
        }
#endif
    private:
        boost::container::flat_map<int64_t, Level, std::greater<int64_t>> _buy, _sell;
};

#endif