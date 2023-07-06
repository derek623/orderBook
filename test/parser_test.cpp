#include "parser_test.hpp"

TEST_P(ParserParameterizedTestFixture, ParserTest)
{
    TestHandler th;    
    Exchange<TestHandler> e(th);
    std::streambuf* sb;
    std::istream i(sb); //dummy istream
    Parser p(i, e);

    ParserParameterizedTestCases c = GetParam();
    for(auto o : c.messages)
    {        
        std::unique_ptr<char[]> buf = std::make_unique<char[]>(std::get<1>(o));
        memcpy(buf.get(), std::get<2>(o), std::get<1>(o));
        p.parse(std::get<0>(o), std::get<1>(o), std::move(buf));        
    }

    EXPECT_EQ(th._ob.getBuy().size(), c.buyLevels.size());
    for (auto [itr1, itr2] = std::tuple{th._ob.getBuy().begin(),c.buyLevels.begin()}; itr2 != c.buyLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }

    EXPECT_EQ(th._ob.getSell().size(), c.sellLevels.size());
    for (auto [itr1, itr2] = std::tuple{th._ob.getSell().begin(),c.sellLevels.begin()}; itr2 != c.sellLevels.end(); ++itr1, ++itr2)
    {
        EXPECT_EQ(itr1->second.price, itr2->price);
        EXPECT_EQ(itr1->second.aggregatedQty, itr2->aggregatedQty);
    }
}

INSTANTIATE_TEST_CASE_P(
        ParserTest,
        ParserParameterizedTestFixture,
        ::testing::Values(
                //Empty
                ParserParameterizedTestCases{{},{},{}},
                //Invalid message size, message will be ignored
                ParserParameterizedTestCases{{{1,33, add1}}, {},{}},
                //Valid cases
                ParserParameterizedTestCases{{{1,32, add1}}, {OrderBook::Level{21,64}},{}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2}}, {OrderBook::Level{110,64}},{}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3}}, {OrderBook::Level{110,64}},{OrderBook::Level{32, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4}}, {OrderBook::Level{110,64}},{OrderBook::Level{4144, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}}, {OrderBook::Level{99,65},OrderBook::Level{89,64}},
                    {OrderBook::Level{4144, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}}, {OrderBook::Level{99,65},OrderBook::Level{89,64}},
                    {OrderBook::Level{4112, -70},OrderBook::Level{88, -71}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}, {7,16,delete1}}, {OrderBook::Level{99,65},OrderBook::Level{89,64}},
                    {OrderBook::Level{4112, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}, {7,16,delete1},{8,16,delete2}}, {OrderBook::Level{99,65}},
                    {OrderBook::Level{4112, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}, {7,16,delete1},{8,16,delete2},{9,24,execute1}}, 
                    {OrderBook::Level{59,65}}, {OrderBook::Level{4112, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}, {7,16,delete1},{8,16,delete2},{9,24,execute1},{10,24,execute2}}, 
                    {}, {OrderBook::Level{4112, -70}}},
                ParserParameterizedTestCases{{{1,32, add1},{2,32, add2},{3,32, add3},{4,32, add4},{5,32, update1}, {6,32,update2}, {7,16,delete1},{8,16,delete2},{9,24,execute1},{10,24,execute2},{11,24,execute3}}, 
                    {}, {}}
        ));