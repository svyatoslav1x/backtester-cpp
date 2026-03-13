#include <gtest/gtest.h>

#include "data_tests.h"

TEST_F(DataHandlerTest, SymbolsAreReturned) {

    const auto symbols = data->get_symbols();

    EXPECT_EQ(symbols.size(), 2);
    EXPECT_EQ(symbols[0], "AAPL");
}

TEST_F(DataHandlerTest, LatestBarsReturnRequestedCount) {

    const auto bars = data->get_latest_bars("AAPL", 3);

    EXPECT_EQ(bars.size(), 10);
}


TEST_F(DataHandlerTest, BarsContainValidPrices) {

    const auto bars = data->get_latest_bars("AAPL", 1);

    ASSERT_FALSE(bars.empty());

    const auto& bar = bars[0];

    EXPECT_GE(bar.high, bar.low);
}