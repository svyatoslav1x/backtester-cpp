#include "macd_tests.h"


TEST_F(MACDTest, FirstBarReturnsNoSignal)
{
    data.push_price("AAPL", 100);

    strategy->calculate_signals(market_event);

    EXPECT_TRUE(events.empty());
}


TEST_F(MACDTest, NoSignalBeforeLongPeriod)
{
    std::cout << "size = " << data.get_latest_bars("AAPL",1).size() << std::endl;
    data.push_price("AAPL", 100);
    strategy->calculate_signals(market_event);

    data.push_price("AAPL", 101);
    strategy->calculate_signals(market_event);

    data.push_price("AAPL",102);
    strategy->calculate_signals(market_event);

    EXPECT_TRUE(events.empty());
}


TEST_F(MACDTest, EMAStartsUpdating)
{
    data.push_price("AAPL",100);
    strategy->calculate_signals(market_event);

    data.push_price("AAPL",110);
    strategy->calculate_signals(market_event);

    double ema = strategy->get_short_ema("AAPL");

    EXPECT_GT(ema, 0.0);
}


TEST_F(MACDTest, GeneratesLongSignal)
{
    strategy = std::make_unique<MovingAveragesLongStrategy>(
        data,
        events,
        portfolio,
        2,
        3
    );

    data.push_price("AAPL",100);
    strategy->calculate_signals(market_event);

    data.push_price("AAPL",105);
    strategy->calculate_signals(market_event);

    data.push_price("AAPL",110);
    strategy->calculate_signals(market_event);

    ASSERT_FALSE(events.empty());

    auto* signal = dynamic_cast<SignalEvent*>(events.front().get());

    ASSERT_NE(signal, nullptr);

    EXPECT_EQ(signal->get_direction(), "LONG");
}
