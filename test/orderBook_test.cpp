#include "orderBook_test.hpp"

TEST_P(AddOrderParameterizedTestFixture, CheckAddOrder)
{
    AddOrderParameterizedTestCases c = GetParam();
    
    for (auto [itr1, itr2] = std::tuple{ c.newOrders.begin(), c.updatedLevels.begin()}; itr2 != c.updatedLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(b.add(std::get<0>(*itr1), std::get<1>(*itr1), std::get<2>(*itr1), std::get<3>(*itr1)), *itr2);
    }

    EXPECT_EQ(b.getBuy().size(), c.buyLevels.size());
    for (auto [itr1, itr2] = std::tuple{b.getBuy().begin(),c.buyLevels.begin()}; itr2 != c.buyLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }

    EXPECT_EQ(b.getSell().size(), c.sellLevels.size());
    for (auto [itr1, itr2] = std::tuple{b.getSell().begin(),c.sellLevels.begin()}; itr2 != c.sellLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }
}

INSTANTIATE_TEST_CASE_P(
        OrderBookCheckAddOrder,
        AddOrderParameterizedTestFixture,
        ::testing::Values(
                //Empty
                AddOrderParameterizedTestCases{{},{},{},{}},
                //Only buy
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,1}}, {0}, {OrderBook::Level{200,1}}, {}},
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,1},{2,Side::Buy,300,2}}, {0,0}, {OrderBook::Level{300,2},OrderBook::Level{200,1}}, {}},
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,2},{2,Side::Buy,300,1}}, {0,1}, {OrderBook::Level{200,2},OrderBook::Level{300,1}}, {}},
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,-2},{2,Side::Buy,300,1},{3,Side::Buy,300,1}},{0,0,0},{OrderBook::Level{600,1},OrderBook::Level{200,-2}}, {}},
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,-2},{2,Side::Buy,300,1},{3,Side::Buy,300,-2}},{0,0,1},{OrderBook::Level{300,1},OrderBook::Level{500,-2}}, {}},
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,10},{2,Side::Buy,300,9},{3,Side::Buy,300,8},{4,Side::Buy,300,7},{5,Side::Buy,300,6}},{0,1,2,3,4},
                    {OrderBook::Level{200,10},OrderBook::Level{300,9},OrderBook::Level{300,8},OrderBook::Level{300,7},OrderBook::Level{300,6}}, {}},
                //Only Sell
                AddOrderParameterizedTestCases{{{1,Side::Sell,200,1}}, {0}, {}, {OrderBook::Level{200,-1}}},
                AddOrderParameterizedTestCases{{{1,Side::Sell,200,1},{2,Side::Sell,300,2}}, {0,1}, {}, {OrderBook::Level{200,-1}, OrderBook::Level{300,-2}}},
                AddOrderParameterizedTestCases{{{1,Side::Sell,200,2},{2,Side::Sell,300,1}}, {0,0}, {}, {OrderBook::Level{300,-1}, OrderBook::Level{200,-2}}},
                AddOrderParameterizedTestCases{{{1,Side::Sell,200,2},{2,Side::Sell,300,-1}}, {0,0}, {}, {OrderBook::Level{300,1}, OrderBook::Level{200,-2}}},
                AddOrderParameterizedTestCases{{{1,Side::Sell,200,-1},{2,Side::Sell,300,2},{3,Side::Sell,300,3},{4,Side::Sell,300,4},{5,Side::Sell,300,5}}, {0,1,2,3,4}, {}, 
                    {OrderBook::Level{200,1}, OrderBook::Level{300,-2}, OrderBook::Level{300,-3}, OrderBook::Level{300,-4}, OrderBook::Level{300,-5}}},
                //both buy and sell
                AddOrderParameterizedTestCases{{{1,Side::Buy,200,-2},{2,Side::Buy,300,1},{3,Side::Buy,300,1},{2,Side::Sell,300,-1},{1,Side::Sell,200,2}}, {0,0,0,0,1}, {OrderBook::Level{600,1},OrderBook::Level{200,-2}},
                    {OrderBook::Level{300,1}, OrderBook::Level{200,-2}}}
        ));

TEST_P(ReduceOrderParameterizedTestFixture, CheckReduceOrder)
{
    ReduceOrderParameterizedTestCases c = GetParam();
    for(auto o : c.newOrders)
    {
        b.add(std::get<0>(o), std::get<1>(o), std::get<2>(o), std::get<3>(o));
    }

    for (auto [itr1, itr2] = std::tuple{ c.reduceOrders.begin(), c.updatedLevels.begin()}; itr1 != c.reduceOrders.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(b.reduce(std::get<0>(*itr1), std::get<1>(*itr1), std::get<2>(*itr1), std::get<3>(*itr1)), *itr2);
    }

    EXPECT_EQ(b.getBuy().size(), c.buyLevels.size());
    for (auto [itr1, itr2] = std::tuple{b.getBuy().begin(),c.buyLevels.begin()}; itr2 != c.buyLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }

    EXPECT_EQ(b.getSell().size(), c.sellLevels.size());
    for (auto [itr1, itr2] = std::tuple{b.getSell().begin(),c.sellLevels.begin()}; itr2 != c.sellLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }
}

INSTANTIATE_TEST_CASE_P(
        OrderBookCheckReduceOrder,
        ReduceOrderParameterizedTestFixture,
        ::testing::Values(
                //Empty
                ReduceOrderParameterizedTestCases{{},{},{},{},{}},
                //Only buy
                ReduceOrderParameterizedTestCases{{{1,Side::Buy,200,1}}, {{2,Side::Buy,200,1}}, {0}, {}, {}},
                ReduceOrderParameterizedTestCases{{{1,Side::Buy,200,1}}, {{2,Side::Buy,100,1}}, {0}, {OrderBook::Level{100,1}}, {}},
                ReduceOrderParameterizedTestCases{{{1,Side::Buy,200,1},{2,Side::Buy,300,-1}}, {{3,Side::Buy,100,1},{4,Side::Buy,300,-1}}, {0, 1}, {OrderBook::Level{100,1}}, {}},
                ReduceOrderParameterizedTestCases{{{1,Side::Buy,200,1},{2,Side::Buy,300,-1}}, {{3,Side::Buy,100,1},{4,Side::Buy,1000,-1}}, {0, 1}, {OrderBook::Level{100,1}}, {}},
                ReduceOrderParameterizedTestCases{{{1,Side::Buy,200,1},{2,Side::Buy,300,-1}}, {{3,Side::Buy,100,1},{4,Side::Buy,150,-1}}, {0, 1}, {OrderBook::Level{100,1},OrderBook::Level{150,-1}}, {}},
                //Only sell
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1}}, {{2,Side::Sell,200,1}}, {0}, {}, {}},
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1}}, {{2,Side::Sell,100,1}}, {0}, {}, {OrderBook::Level{100,-1}}},
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1},{2,Side::Sell,300,-2}}, {{3,Side::Sell,100,1},{4,Side::Sell,300,-2}}, {1,0}, {}, {OrderBook::Level{100,-1}}},
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1},{2,Side::Sell,300,-2}}, {{3,Side::Sell,100,1},{4,Side::Sell,1000,-2}}, {1,0}, {}, {OrderBook::Level{100,-1}}},
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1},{2,Side::Sell,300,-2}}, {{3,Side::Sell,100,1},{4,Side::Sell,150,-2}}, {1,0}, {}, {OrderBook::Level{150,2},OrderBook::Level{100,-1}}},
                //Both buy and sell
                ReduceOrderParameterizedTestCases{{{1,Side::Sell,200,1},{2,Side::Sell,300,-2},{1,Side::Buy,200,1},{2,Side::Buy,300,-1}}, {{3,Side::Buy,100,1},{4,Side::Buy,150,-1},{3,Side::Sell,100,1},{4,Side::Sell,150,-2}}, {0,1,1,0}, 
                    {OrderBook::Level{100,1},OrderBook::Level{150,-1}}, {OrderBook::Level{150,2},OrderBook::Level{100,-1}}}
        ));