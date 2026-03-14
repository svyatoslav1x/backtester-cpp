#include <cmath>
#include <gtest/gtest.h>
#include <vector>

#include "../include/performance.h"

TEST(PerformanceTest, SharpeRatioEmptyVector) {
	std::vector<double> returns = {};
	double sharpe = performance::calculate_sharpe_ratio(returns);
	EXPECT_DOUBLE_EQ(sharpe, 0.0);
}

TEST(PerformanceTest, SharpeRatioZeroVariance) {
	// if returns are perfectly constant, standard deviation is 0,
	// the function must safely return 0.0 without dividing by zero.
	std::vector<double> returns = {0.05, 0.05, 0.05, 0.05};
	double sharpe = performance::calculate_sharpe_ratio(returns);
	EXPECT_DOUBLE_EQ(sharpe, 0.0);
}

TEST(PerformanceTest, SharpeRatioNormalCalculation) {
	std::vector<double> returns = {0.01, 0.02, 0.03};

	// mean = 0.02
	// deviations from mean: -0.01, 0, 0.01
	// variance (population) = (0.0001 + 0 + 0.0001) / 3 = 0.000066666...
	// stddev = sqrt(0.000066666...) ≈ 0.0081649658
	// sharpe = (sqrt(252) * 0.02) / 0.0081649658 ≈ 38.884499

	double sharpe = performance::calculate_sharpe_ratio(returns, 252.0);

	EXPECT_NEAR(sharpe, 38.884499, 0.001);
}

TEST(PerformanceTest, SharpeRatioNegativeReturns) {
	// ff the strategy loses money, the Sharpe ratio should be negative
	std::vector<double> returns = {-0.01, -0.02, -0.03};
	double sharpe = performance::calculate_sharpe_ratio(returns, 252.0);
	EXPECT_LT(sharpe, 0.0); // Less Than 0
}

TEST(PerformanceTest, DrawdownsEmptyVector) {
	std::vector<double> curve = {};
	auto [max_dd, max_dur] = performance::calculate_drawdowns(curve);

	EXPECT_DOUBLE_EQ(max_dd, 0.0);
	EXPECT_EQ(max_dur, 0);
}

TEST(PerformanceTest, DrawdownsAlwaysIncreasing) {
	// if the equity curve only goes up, there is never a drawdown
	std::vector<double> curve = {100.0, 110.0, 120.0, 150.0};
	auto [max_dd, max_dur] = performance::calculate_drawdowns(curve);

	EXPECT_DOUBLE_EQ(max_dd, 0.0);
	EXPECT_EQ(max_dur, 0);
}

TEST(PerformanceTest, DrawdownsWithRecovery) {
	// curve: starts at 100, peaks at 110, drops to 90, drops to 80, recovers to 120
	std::vector<double> curve = {100.0, 110.0, 90.0, 80.0, 120.0};

	// High Water Mark starts at 100.
	// 110: HWM = 110. DD = 0. Dur = 0.
	// 90:  HWM = 110. DD = 20. Dur = 1.
	// 80:  HWM = 110. DD = 30. Dur = 2.
	// 120: HWM = 120. DD = 0. Dur = 0.

	auto [max_dd, max_dur] = performance::calculate_drawdowns(curve);

	EXPECT_DOUBLE_EQ(max_dd, 30.0); // 110 - 80 = 30
	EXPECT_EQ(max_dur, 2); // spent 2 periods below the 110 peak
}

TEST(PerformanceTest, DrawdownsContinuousDrop) {
	// curve that strictly loses money
	std::vector<double> curve = {100.0, 90.0, 80.0, 70.0};

	// HWM stays at 100 the whole time
	// peak drop is at the end (100 - 70 = 30).
	// duration should be 3 periods.

	auto [max_dd, max_dur] = performance::calculate_drawdowns(curve);

	EXPECT_DOUBLE_EQ(max_dd, 30.0);
	EXPECT_EQ(max_dur, 3);
}
