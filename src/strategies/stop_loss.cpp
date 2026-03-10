#include "stop_loss.h"
#include <cmath>

StopLossStrategy::StopLossStrategy(DataHandler& data_,
								   std::queue<std::unique_ptr<Event>>& events_,
								   Portfolio& portfolio_,
								   double stop_loss_percentage_) :
	data(data_), events(events_), portfolio(portfolio_), stop_loss_percentage(stop_loss_percentage_) {
	symbol_list = data.get_symbols();

	for (const auto& symbol : symbol_list) {
		bought[symbol] = false;
		stop_loss[symbol] = stop_loss_percentage;
	}
}

void StopLossStrategy::calculate_signals(const Event& event) {
	if (event.type() != EventType::MARKET)
		return;

	for (const auto& symbol : symbol_list) {
		// fetch the last 2 bars (required for trailing stop logic)
		auto bars = data.get_latest_bars(symbol, 2);

		if (bars.empty())
			continue;

		double latest_close = bars.back().close;
		auto date = bars.back().datetime;

		// Condition to buy: if we haven't bought yet and price > (initial_stop / percentage)
		// With stop_loss_percentage = 0.9, 0.9 / 0.9 = 1.0. Buys on first tick > $1.0.
		if (!bought[symbol] && latest_close > (stop_loss[symbol] / stop_loss_percentage)) {
			int quantity = static_cast<int>(std::floor(portfolio.get_cash() / latest_close));

			if (quantity > 0) {
				events.push(std::make_unique<SignalEvent>(symbol, date, "LONG", quantity));
				bought[symbol] = true;

				// set the actual dollar-value trailing stop level
				stop_loss[symbol] = stop_loss_percentage * latest_close;
			}
		} else if (bought[symbol]) {
			// check if price fell below the stop loss level
			if (latest_close <= stop_loss[symbol]) {
				int quantity = portfolio.get_position(symbol);
				events.push(std::make_unique<SignalEvent>(symbol, date, "EXIT", quantity));
				bought[symbol] = false;

				// reset to percentage for the next potential entry
				stop_loss[symbol] = stop_loss_percentage;
			}
			// Trailing Stop Logic: Check if price went up compared to previous bar
			else {
				if (bars.size() >= 2) {
					double prev_close = bars[0].close;
					double current_close = bars[1].close;

					// if the price increased, calculate the new potential stop level
					double potential_new_stop = stop_loss_percentage * current_close;

					// ratchet the stop loss upward (it can never go down while holding)
					if (current_close > prev_close && potential_new_stop > stop_loss[symbol]) {
						stop_loss[symbol] = potential_new_stop;
					}
				}
			}
		}
	}
}

void StopLossStrategy::plot() {}

std::map<std::string, double> StopLossStrategy::get_indicators() const {
	std::map<std::string, double> indicators;
	std::string symbol = symbol_list[0];
	if (bought.at(symbol)) {
		indicators["Stop Level"] = stop_loss.at(symbol);
	}
	return indicators;
}
