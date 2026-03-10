#include "../../include/performance.h"
#include <algorithm>
#include <cmath>
#include <numeric>

namespace performance {

double calculate_sharpe_ratio(const std::vector<double>& returns, double periods) {
	if (returns.empty())
		return 0.0;

	double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
	double mean = sum / static_cast<double>(returns.size());

	double sq_sum = 0.0;
	for (double r : returns) {
		sq_sum += (r - mean) * (r - mean);
	}

	double std_dev = std::sqrt(sq_sum / static_cast<double>(returns.size()));

	if (std_dev == 0.0)
		return 0.0;

	return (std::sqrt(periods) * mean) / std_dev;
}

std::pair<double, int> calculate_drawdowns(const std::vector<double>& equity_curve) {
	if (equity_curve.empty())
		return {0.0, 0};

	double high_water_mark = equity_curve[0];
	double max_drawdown = 0.0;

	int current_duration = 0;
	int max_duration = 0;

	for (std::size_t i = 1; i < equity_curve.size(); ++i) {
		// update high water mark
		high_water_mark = std::max(high_water_mark, equity_curve[i]);

		// drawdown is the distance below the HWM
		double drawdown = high_water_mark - equity_curve[i];
		max_drawdown = std::max(max_drawdown, drawdown);

		// if we are below the HWM, increment duration
		if (drawdown > 0.0) {
			current_duration++;
			max_duration = std::max(max_duration, current_duration);
		} else {
			current_duration = 0;
		}
	}

	return {max_drawdown, max_duration};
}

} // namespace performance
