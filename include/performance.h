// When carrying out an algorithmic trading strategy it is tempting to consider the annualised return as the
// most useful performance metric. However, there are many flaws with using this measure in isolation.
//  The calculation of returns for certain strategies is not completely straightforward.
//  This is especially true for strategies that aren't directional such as market-neutral variants or
//  strategies which make use of leverage. These factors make it hard to compare two strategies based solely
//  upon their returns.
//
// In addition, if we are presented with two strategies possessing identical returns how do we know which one
// contains more risk ?
//  Further, what do we even mean by "more risk"? In finance, we are often concerned with volatility of
//  returns and periods of drawdown. Thus if one of these strategies has a significantly higher volatility of
//  returns we would likely find it less attractive, despite the fact that its historical returns might be
//  similar if not identical.

#pragma once

#include <cstddef>
#include <utility>
#include <vector>

// namespace here is used for more modular organization of performance-related calculations
namespace performance {

// Calculates the Sharpe Ratio for the strategy.
// 'returns' is a vector of period percentage returns.
// 'periods' is the number of trading periods in a year (e.g., 252 for daily).
// It returns the annualized Sharpe Ratio.
double calculate_sharpe_ratio(const std::vector<double>& returns, double periods = 252.0);

// Calculates the maximum drawdown and maximum drawdown duration.
// 'equity_curve' is a vector of the total account value at each time step.
// It returns a pair containing {max_drawdown, max_duration}.
std::pair<double, int> calculate_drawdowns(const std::vector<double>& equity_curve);

} // namespace performance
