#ifndef DISPLAYHANDLER
#define DISPLAYHANDLER


#include "orderBook.hpp"

class DisplayHandler {
    //onQuote and onTrade have the same behaviour in this test, but in real life they should behave differently
    public:
        DisplayHandler(const uint32_t displayLevel) : _displayLevel{displayLevel}{}
        void onQuote(const uint32_t& seqno, const std::string_view& symbol, const OrderBook& ob, int64_t lastUpdatedDepth){
            onUpdate(seqno, symbol, ob, lastUpdatedDepth);
        }
        void onTrade(const uint32_t& seqno, const std::string_view& symbol, const OrderBook& ob, int64_t lastUpdatedDepth){
            onUpdate(seqno, symbol, ob, lastUpdatedDepth);
        }

    private:
        void onUpdate(const uint32_t& seqno, const std::string_view& symbol, const OrderBook& ob, int64_t lastUpdatedDepth){
            if(lastUpdatedDepth == std::numeric_limits<uint64_t>::max())
                return;

            //lastUpdatedDepth is 0 indexed
            if(lastUpdatedDepth < _displayLevel)
            {
                std::cout << seqno << ", " << symbol << ", "; 
                ob.print(_displayLevel);
            }
        }
        uint64_t _displayLevel;
};

#endif