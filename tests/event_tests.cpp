#include "event_tests.h"
#include <sstream>
#include <cstdio>

//  MarketEvent Tests 
TEST_F(EventTest, MarketEventFields) {
    MarketEvent m;
    EXPECT_EQ(m.type(), EventType::MARKET);
    EXPECT_EQ(m.get_symbol(), "");
    EXPECT_EQ(m.get_quantity(), 0);
    EXPECT_EQ(m.get_direction(), "");
}

//SignalEvent Tests
TEST_F(EventTest, SignalEventInitialization) {
    SignalEvent s("AAPL", now, "LONG", 100);

    EXPECT_EQ(s.type(), EventType::SIGNAL);
    EXPECT_EQ(s.get_symbol(), "AAPL");
    EXPECT_EQ(s.get_direction(), "LONG");
    EXPECT_EQ(s.get_quantity(), 100);
}

//OrderEvent Tests
TEST_F(EventTest, OrderEventInitialization) {
    OrderEvent o("GOOG", "MKT", 50, "BUY");

    EXPECT_EQ(o.type(), EventType::ORDER);
    EXPECT_EQ(o.get_symbol(), "GOOG");
    EXPECT_EQ(o.get_direction(), "BUY");
    EXPECT_EQ(o.get_quantity(), 50);
    EXPECT_EQ(o.get_direction(), "BUY");
}

TEST_F(EventTest, OrderEventPrintOrder) {
    OrderEvent o("MSFT", "LMT", 10, "SELL");

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "Order: Symbol=%s, Type=%s, Quantity=%d, Direction=%s\n",
                  o.get_symbol().c_str(), "LMT", o.get_quantity(), o.get_direction().c_str());

    testing::internal::CaptureStdout();
    o.print_order();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, buffer);
}

// FillEvent Tests
TEST_F(EventTest, FillEventInitializationWithAutoCommission) {
    FillEvent f(now, "AAPL", "NYSE", 100, "BUY", 150.0, -1.0);

    EXPECT_EQ(f.type(), EventType::FILL);
    EXPECT_EQ(f.get_symbol(), "AAPL");
    EXPECT_EQ(f.get_direction(), "BUY");
    EXPECT_EQ(f.get_quantity(), 100);
    EXPECT_EQ(f.get_fill_cost(), 150.0);
    EXPECT_GT(f.get_commission(), 0.0); // комиссия рассчитана автоматически
}

TEST_F(EventTest, FillEventInitializationWithGivenCommission) {
    FillEvent f(now, "TSLA", "NASDAQ", 200, "SELL", 200.0, 5.0);

    EXPECT_EQ(f.get_commission(), 5.0); // комиссия передана явно
}

TEST_F(EventTest, FillEventCalculateIBCommission) {
    FillEvent f1(now, "AAPL", "NYSE", 100, "BUY", 10.0, 0.0);
    FillEvent f2(now, "AAPL", "NYSE", 600, "BUY", 10.0, 0.0);

    double comm1 = f1.calculate_ib_commission();
    double comm2 = f2.calculate_ib_commission();

    EXPECT_GE(comm1, 1.3);
    EXPECT_GE(comm2, 1.3);
}