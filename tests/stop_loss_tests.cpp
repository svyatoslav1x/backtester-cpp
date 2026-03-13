#include "stop_loss_tests.h"

TEST_F(LossTest, NoSignalOnFirstBar)
{
    data.push_price("AAPL", 0.5);

    MarketEvent event;
    strategy->calculate_signals(event);

    EXPECT_TRUE(events.empty());
}

TEST_F(LossTest, GeneratesLongSignal)
{
    data.push_price("AAPL", 10.0);

    MarketEvent event;
    strategy->calculate_signals(event);

    ASSERT_FALSE(events.empty());

    auto* signal = dynamic_cast<SignalEvent*>(events.front().get());

    ASSERT_NE(signal, nullptr);
    EXPECT_EQ(signal->get_symbol(), "AAPL");
    EXPECT_EQ(signal->get_direction(), "LONG");
}

TEST_F(LossTest, GeneratesExitSignal)
{
    data.push_price("AAPL", 10.0);

    MarketEvent event;
    strategy->calculate_signals(event);

    events = {};

    data.push_price("AAPL", 8.0);

    strategy->calculate_signals(event);

    ASSERT_FALSE(events.empty());

    auto* signal = dynamic_cast<SignalEvent*>(events.front().get());

    ASSERT_NE(signal, nullptr);
    EXPECT_EQ(signal->get_direction(), "EXIT");
}

TEST_F(LossTest, TrailingStopMovesUp)
{
    data.push_price("AAPL", 10.0);

    MarketEvent event;
    strategy->calculate_signals(event);

    events = {};

    data.push_price("AAPL", 12.0);
    strategy->calculate_signals(event);

    auto indicators = strategy->get_indicators();

    ASSERT_TRUE(indicators.count("Stop Level") > 0);

    double stop = indicators["Stop Level"];

    EXPECT_GT(stop, 9.0);
}

TEST_F(LossTest, StopNeverMovesDown)
{
    data.push_price("AAPL", 10.0);

    MarketEvent event;
    strategy->calculate_signals(event);

    events = {};

    data.push_price("AAPL", 12.0);
    strategy->calculate_signals(event);

    double stop1 = strategy->get_indicators()["Stop Level"];

    data.push_price("AAPL", 11.0);
    strategy->calculate_signals(event);

    double stop2 = strategy->get_indicators()["Stop Level"];

    EXPECT_GE(stop2, stop1);
}