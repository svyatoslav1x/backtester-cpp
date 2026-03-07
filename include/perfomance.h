//When carrying out an algorithmic trading strategy it is tempting to consider the annualised return as the most useful performance metric.However, there are many flaws with using this measure in isolation.
// The calculation of returns for certain strategies is not completely straightforward.
// This is especially true for strategies that aren't directional such as market-neutral variants or strategies which make use of leverage. 
// These factors make it hard to compare two strategies based solely upon their returns.
//
//In addition, if we are presented with two strategies possessing identical returns how do we know which one contains more risk ? 
// Further, what do we even mean by "more risk" ? In finance, we are often concerned with volatility of returns and periods of drawdown.
// Thus if one of these strategies has a significantly higher volatility of returns we would likely find it less attractive, despite the fact that its historical returns might be similar if not identical.



#pragma once

#include <vector>
#include <utility>
#include "data.h"

// stores the functions to calculate the Sharpe ratio and drawdown information


#include "performance.hpp"

#include <cmath>
#include <numeric>
#include <algorithm>
#include <stdexcept>

namespace performance {

    double create_sharpe_ratio(
        const std::vector<double>& strategy_returns,
        const std::vector<double>& benchmark_returns,
        double periods
    ) {
        if (strategy_returns.size() != benchmark_returns.size()) {
            throw std::invalid_argument(
                "Strategy and benchmark return vectors must have equal size."
            );
        }

        const std::size_t n = strategy_returns.size();
        if (n == 0) return 0.0;

        // Compute excess returns
        std::vector<double> excess_returns;
        excess_returns.reserve(n);

        for (std::size_t i = 0; i < n; ++i) {
            excess_returns.push_back(
                strategy_returns[i] - benchmark_returns[i]
            );
        }

        // Mean of excess returns
        double mean = std::accumulate(
            excess_returns.begin(),
            excess_returns.end(),
            0.0
        ) / static_cast<double>(n);

        // Variance of excess returns
        double sq_sum = 0.0;
        for (double r : excess_returns) {
            sq_sum += (r - mean) * (r - mean);
        }

        double variance = sq_sum / static_cast<double>(n);
        double stddev = std::sqrt(variance);

        if (stddev == 0.0) return 0.0;

        return std::sqrt(periods) * mean / stddev;
    }


    // Drawdown & duration

    std::pair<double, std::size_t> create_drawdowns(
        const std::vector<double>& equity_curve
    ) {
        if (equity_curve.empty()) {
            return { 0.0, 0 };
        }

        double high_water_mark = equity_curve[0];
        double max_drawdown = 0.0;

        std::size_t duration = 0;
        std::size_t max_duration = 0;

        for (std::size_t i = 1; i < equity_curve.size(); ++i) {

            high_water_mark = std::max(
                high_water_mark,
                equity_curve[i]
            );

            double drawdown = high_water_mark - equity_curve[i];

            if (drawdown > 0.0) {
                ++duration;
                max_duration = std::max(max_duration, duration);
            }
            else {
                duration = 0;
            }

            max_drawdown = std::max(max_drawdown, drawdown);
        }

        return { max_drawdown, max_duration };
    }

}

