#include "macd_tests.h"
#include <chrono>
#include <cmath>

// ---------------- MovingAveragesLongStrategy Tests ----------------
TEST(MACDTest, LongStrategyEMAUpdate) {
    MockDataHandler data;
    std::queue<std::unique_ptr<Event>> events;
    MockPortfolio portfolio;

    MovingAveragesLongStrategy strat(data, events, portfolio, 2, 3);

    // Добавляем бары
    auto now = std::chrono::system_clock::now();
    data.bars["AAPL"].push_back({now, 10.0});
    data.bars["AAPL"].push_back({now, 12.0});
    data.bars["AAPL"].push_back({now, 14.0});

    // Создаём MarketEvent
    MarketEvent me;
    strat.calculate_signals(me);

    // Проверяем что EMA обновились
    double short_ema = strat.get_short_ema("AAPL");
    double long_ema = strat.get_long_ema("AAPL");
    EXPECT_GT(short_ema, 0.0);
    EXPECT_GT(long_ema, 0.0);

    // После достаточного количества баров должна быть сгенерирована LONG signal
    EXPECT_FALSE(events.empty());
    auto& ev = events.front();
    EXPECT_EQ(ev->type(), EventType::SIGNAL);
    EXPECT_EQ(ev->get_symbol(), "AAPL");
    EXPECT_EQ(ev->get_direction(), "LONG");
}

// ---------------- MovingAveragesLongShortStrategy Tests ----------------
TEST(MACDTest, LongShortStrategySignalGeneration) {
    MockDataHandler data;
    std::queue<std::unique_ptr<Event>> events;
    MockPortfolio portfolio;
    portfolio.positions["AAPL"] = 0;

    MovingAveragesLongShortStrategy strat(data, events, portfolio, 2, 3);

    auto now = std::chrono::system_clock::now();
    data.bars["AAPL"].push_back({now, 10.0});
    data.bars["AAPL"].push_back({now, 12.0});
    data.bars["AAPL"].push_back({now, 14.0});

    MarketEvent me;
    strat.calculate_signals(me);

    EXPECT_FALSE(events.empty());
    auto& ev = events.front();
    EXPECT_EQ(ev->type(), EventType::SIGNAL);
    EXPECT_EQ(ev->get_symbol(), "AAPL");
}

// ---------------- MovingAveragesMomentumStrategy Tests ----------------
TEST(MACDTest, MomentumStrategyPositionSizing) {
    MockDataHandler data;
    std::queue<std::unique_ptr<Event>> events;
    MockPortfolio portfolio;
    portfolio.positions["AAPL"] = 10;

    MovingAveragesMomentumStrategy strat(data, events, portfolio, 2, 3);

    auto now = std::chrono::system_clock::now();
    data.bars["AAPL"].push_back({now, 10.0});
    data.bars["AAPL"].push_back({now, 12.0});
    data.bars["AAPL"].push_back({now, 14.0});

    MarketEvent me;
    strat.calculate_signals(me);

    EXPECT_FALSE(events.empty());
    auto& ev = events.front();
    EXPECT_EQ(ev->type(), EventType::SIGNAL);
    EXPECT_EQ(ev->get_symbol(), "AAPL");
}