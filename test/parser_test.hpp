#include "gtest/gtest.h"
#include "parser.hpp"
#include <vector>

struct TestHandler {
    public:
        TestHandler(){}
        void onQuote(const uint32_t& seqno, const std::string_view& symbol, const OrderBook& ob, int64_t lastUpdatedDepth){
            _ob = ob;
        }
        void onTrade(const uint32_t& seqno, const std::string_view& symbol, const OrderBook& ob, int64_t lastUpdatedDepth){
            _ob = ob;
        }

        OrderBook _ob;
};

struct ParserParameterizedTestCases {
    //seqno, bOrS, qty, price
    std::vector<std::tuple<uint32_t,uint32_t,char*>> messages;
    std::vector<OrderBook::Level> buyLevels;
    std::vector<OrderBook::Level> sellLevels;
};

class ParserParameterizedTestFixture : public ::testing::TestWithParam<ParserParameterizedTestCases> {
    /*protected:
        OrderBook b;*/
};

char add1[] = { 'A', 'V', 'C', 'O', 01, 0, 0, 0, 0, 0, 0, 0, 'B', 20, 20, 20, 21, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 20, 20, 20, 20};
char add2[] = { 'A', 'V', 'C', 'O', 02, 0, 0, 0, 0, 0, 0, 0, 'B', 20, 20, 20, 89, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 20, 20, 20, 20};
char add3[] = { 'A', 'V', 'C', 'O', 03, 0, 0, 0, 0, 0, 0, 0, 'S', 20, 20, 20, 32, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 20, 20, 20, 20};
char add4[] = { 'A', 'V', 'C', 'O', 04, 0, 0, 0, 0, 0, 0, 0, 'S', 20, 20, 20, 0x10, 0x10, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 20, 20, 20, 20};
char update1[] = { 'U', 'V', 'C', 'O', 01, 0, 0, 0, 0, 0, 0, 0, 'B', 20, 20, 20, 99, 0, 0, 0, 0, 0, 0, 0, 65, 0, 0, 0, 20, 20, 20, 20};
char update2[] = { 'U', 'V', 'C', 'O', 03, 0, 0, 0, 0, 0, 0, 0, 'S', 20, 20, 20, 88, 0, 0, 0, 0, 0, 0, 0, 71, 0, 0, 0, 20, 20, 20, 20};
char delete1[] = { 'D', 'V', 'C', 'O', 03, 0, 0, 0, 0, 0, 0, 0, 'S'};
char delete2[] = { 'D', 'V', 'C', 'O', 02, 0, 0, 0, 0, 0, 0, 0, 'B'};
char execute1[] = { 'E', 'V', 'C', 'O', 01, 0, 0, 0, 0, 0, 0, 0, 'B', 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0};
char execute2[] = { 'E', 'V', 'C', 'O', 01, 0, 0, 0, 0, 0, 0, 0, 'B', 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 0};
char execute3[] = { 'E', 'V', 'C', 'O', 04, 0, 0, 0, 0, 0, 0, 0, 'B', 0, 0, 0, 0x10, 0x10, 0, 0, 0, 0, 0, 0};