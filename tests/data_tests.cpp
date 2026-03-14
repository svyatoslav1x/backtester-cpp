#include "data_tests.h"

// после создания handler нет данных в latest_symbol_data
TEST_F(DataHandlerTest, InitiallyEmptyLatestBars) {
    auto bars = handler->get_latest_bars("TEST", 1);
    EXPECT_TRUE(bars.empty());
}

// update_bars: добавление 1 бара
TEST_F(DataHandlerTest, UpdateBarsAddsOneBar) {
    handler->update_bars();
    auto bars = handler->get_latest_bars("TEST", 1);
    ASSERT_EQ(bars.size(), 1);
    EXPECT_EQ(bars[0].close, 11);
}

// get_latest_bars с N > 1
TEST_F(DataHandlerTest, GetLatestNBars) {
    handler->update_bars(); // 1
    handler->update_bars(); // 2
    auto bars = handler->get_latest_bars("TEST", 2);
    ASSERT_EQ(bars.size(), 2);
    EXPECT_EQ(bars[0].close, 11);
    EXPECT_EQ(bars[1].close, 12);
}

// if_continue_backtest
TEST_F(DataHandlerTest, ContinueBacktestFlag) {
    EXPECT_TRUE(handler->if_continue_backtest());
    handler->update_bars();
    handler->update_bars();
    handler->update_bars();
    handler->update_bars();
    EXPECT_FALSE(handler->if_continue_backtest());
}

//get_symbols
TEST_F(DataHandlerTest, GetSymbolsReturnsList) {
    auto s = handler->get_symbols();
    ASSERT_EQ(s.size(), 1);
    EXPECT_EQ(s[0], "TEST");
}

// MarketEvent в очередь
TEST_F(DataHandlerTest, UpdateBarsPushesEvent) {
    size_t initial_size = events.size();
    handler->update_bars();
    EXPECT_EQ(events.size(), initial_size + 1);
    // Проверяем тип события через dynamic_cast
    auto& evt = events.back();
    EXPECT_NE(dynamic_cast<MarketEvent*>(evt.get()), nullptr);
}