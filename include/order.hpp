#ifndef __Order_HPP__
#define __Order_HPP__

constexpr static uint16_t DEFAULT_BOOKID = std::numeric_limits<uint16_t>::max();        

enum class Side {
    Buy = 0,
    Sell,
    Unknown
};

struct Order {
    int32_t price = 0;
    uint64_t qty = 0;
    Side buy_sell = Side::Unknown;
    uint16_t bookid = DEFAULT_BOOKID;

    Order(int32_t price, uint64_t qty, Side buy_sell, uint16_t bookid)
        : price(price), qty(qty), buy_sell(buy_sell), bookid(bookid) {}
    Order() {}

    friend std::ostream& operator<<(std::ostream& out, const Order& o)
    {
        char bs = Side::Buy == o.buy_sell ? 'B' : 'S';
        out << " Order: " << o.price << ", " << o.qty << ", " << bs << ", " << o.bookid;
        return out;
    }
};

#endif