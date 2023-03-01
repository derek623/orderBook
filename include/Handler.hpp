#ifndef DISPLAYHANDLER
#define DISPLAYHANDLER


#include "orderBook.hpp"

//Possibly the handler class is a virtual base class as it really depends how the onQuote and onTrade is being handled
class DisplayHandler {
    public:
        DisplayHandler(const uint32_t& displayLevel) : _displayLevel{_displayLevel}{}
        void onQuote(const OrderBook& ob, int64_t lastUpdatedDepth){
            onUpdate(ob, lastUpdatedDepth);
        }
        void onTrade(const OrderBook& ob, int64_t lastUpdatedDepth){
            onUpdate(ob, lastUpdatedDepth);
        }

        void onUpdate(const OrderBook& ob, int64_t lastUpdatedDepth){
            if(lastUpdatedDepth == std::numeric_limits<uint64_t>::max())
                return;
        }
    private:
        uint32_t _displayLevel;
};

#endif