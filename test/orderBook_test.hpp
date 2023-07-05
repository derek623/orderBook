#include "gtest/gtest.h"
#include "orderBook.hpp"
#include <vector>

struct AddOrderParameterizedTestCases {
    //seqno, bOrS, qty, price
    std::vector<std::tuple<uint32_t, Side, uint64_t, int32_t>> newOrders;
    std::vector<int> updatedLevels;
    std::vector<OrderBook::Level> buyLevels;
    std::vector<OrderBook::Level> sellLevels;
};

class AddOrderParameterizedTestFixture : public ::testing::TestWithParam<AddOrderParameterizedTestCases> {
    protected:
        OrderBook b;
};

struct ReduceOrderParameterizedTestCases {
    //seqno, bOrS, qty, price
    std::vector<std::tuple<uint32_t, Side, uint64_t, int32_t>> newOrders;
    std::vector<std::tuple<uint32_t, Side, uint64_t, int32_t>> reduceOrders;
    std::vector<int> updatedLevels;
    std::vector<OrderBook::Level> buyLevels;
    std::vector<OrderBook::Level> sellLevels;
};

class ReduceOrderParameterizedTestFixture : public ::testing::TestWithParam<ReduceOrderParameterizedTestCases> {
    protected:
        OrderBook b;
};