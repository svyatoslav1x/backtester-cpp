#include "macd.h"
#include <cmath>
#include <numbers>

const double PI = 3.14159265358979323846;

MovingAveragesLongStrategy::MovingAveragesLongStrategy(DataHandler& data,
	std::queue<std::unique_ptr<Event>>& events,
	Portfolio& portfolio,
	int short_period,
	int long_period) :
	data(data), events(events), portfolio(portfolio), short_period(short_period), long_period(long_period) {
	symbol_list = data.get_symbols();
	for (const auto& symbol : symbol_list) {
		bought[symbol] = false;
		bars_processed[symbol] = 0;
		short_ema[symbol] = 0.0;
		long_ema[symbol] = 0.0;
	}
}

void MovingAveragesLongStrategy::update_ema(const std::string& symbol, double price) {
	bars_processed.at(symbol)++;

	if (bars_processed.at(symbol) == 1) { // 
		short_ema.at(symbol) = price;
		long_ema.at(symbol) = price;
	} else {
		double alpha_short = 2.0 / (short_period + 1.0);
		double alpha_long = 2.0 / (long_period + 1.0);

		short_ema.at(symbol) = (price * alpha_short) + (short_ema.at(symbol) * (1.0 - alpha_short));
		long_ema.at(symbol) = (price * alpha_long) + (long_ema.at(symbol) * (1.0 - alpha_long));
	}
}

void MovingAveragesLongStrategy::calculate_signals(const Event& event) {
	if (event.type() != EventType::MARKET)
		return;

	for (const auto& symbol : symbol_list) {
		auto bars = data.get_latest_bars(symbol, 1);
		if (bars.empty())
			continue;

		double price = bars[0].close;
		auto date = bars[0].datetime;

		update_ema(symbol, price);

		if (bars_processed.at(symbol) < long_period)
			continue;

		double s_ema = short_ema.at(symbol);
		double l_ema = long_ema.at(symbol);

		if (!bought.at(symbol) && s_ema > l_ema) {
			int quantity = static_cast<int>(std::floor(portfolio.get_cash() / price));
			events.push(std::make_unique<SignalEvent>(symbol, date, "LONG", quantity));
			bought.at(symbol) = true;
		} else if (bought.at(symbol) && s_ema < l_ema) {
			int quantity = portfolio.get_position(symbol);
			events.push(std::make_unique<SignalEvent>(symbol, date, "EXIT", quantity));
			bought.at(symbol) = false;
		}
	}
}

void MovingAveragesLongStrategy::plot() {}

std::map<std::string, double> MovingAveragesLongStrategy::get_indicators() const {
	std::map<std::string, double> indicators;

	// symbol_list[0] is the active ticker
	std::string symbol = symbol_list[0];

	// only return values if we have actually started calculating them
	if (bars_processed.at(symbol) >= short_period) {
		indicators["Short EMA"] = short_ema.at(symbol);
	}
	if (bars_processed.at(symbol) >= long_period) {
		indicators["Long EMA"] = long_ema.at(symbol);
	}

	return indicators;
}

MovingAveragesLongShortStrategy::MovingAveragesLongShortStrategy(DataHandler& data,
	std::queue<std::unique_ptr<Event>>& events,
	Portfolio& portfolio,
	int short_period,
	int long_period) :
	data(data), events(events), portfolio(portfolio), short_period(short_period), long_period(long_period) {
	symbol_list = data.get_symbols();
	for (const auto& symbol : symbol_list) {
		bought[symbol] = false;
		bars_processed[symbol] = 0;
		short_ema[symbol] = 0.0;
		long_ema[symbol] = 0.0;
	}
}

void MovingAveragesLongShortStrategy::update_ema(const std::string& symbol, double price) {
	bars_processed.at(symbol)++;

	if (bars_processed.at(symbol) == 1) {
		short_ema.at(symbol) = price;
		long_ema.at(symbol) = price;
	} else {
		double alpha_short = 2.0 / (short_period + 1.0);
		double alpha_long = 2.0 / (long_period + 1.0);

		short_ema.at(symbol) = (price * alpha_short) + (short_ema.at(symbol) * (1.0 - alpha_short));
		long_ema.at(symbol) = (price * alpha_long) + (long_ema.at(symbol) * (1.0 - alpha_long));
	}
}

void MovingAveragesLongShortStrategy::calculate_signals(const Event& event) {
	if (event.type() != EventType::MARKET)
		return;

	for (const auto& symbol : symbol_list) {
		auto bars = data.get_latest_bars(symbol, 1);
		if (bars.empty())
			continue;

		double price = bars[0].close;
		auto date = bars[0].datetime;

		update_ema(symbol, price);

		if (bars_processed.at(symbol) < long_period)
			continue;

		double s_ema = short_ema.at(symbol);
		double l_ema = long_ema.at(symbol);
		int current_positions = portfolio.get_position(symbol);

		if (!bought.at(symbol) && s_ema > l_ema) {
			int quantity = static_cast<int>(std::floor(portfolio.get_cash() / price)) + current_positions;

			events.push(std::make_unique<SignalEvent>(symbol, date, "EXIT", std::abs(current_positions)));
			events.push(std::make_unique<SignalEvent>(symbol, date, "LONG", quantity));
			bought.at(symbol) = true;
		} else if (bought.at(symbol) && s_ema < l_ema) {
			int quantity = current_positions;

			events.push(std::make_unique<SignalEvent>(symbol, date, "EXIT", quantity));
			events.push(std::make_unique<SignalEvent>(symbol, date, "SHORT", quantity));
			bought.at(symbol) = false;
		}
	}
}

void MovingAveragesLongShortStrategy::plot() {}

std::map<std::string, double> MovingAveragesLongShortStrategy::get_indicators() const {
	std::map<std::string, double> indicators;
	std::string symbol = symbol_list[0];

	if (bars_processed.at(symbol) >= short_period) {
		indicators["Short EMA"] = short_ema.at(symbol);
	}
	if (bars_processed.at(symbol) >= long_period) {
		indicators["Long EMA"] = long_ema.at(symbol);
	}

	return indicators;
}

MovingAveragesMomentumStrategy::MovingAveragesMomentumStrategy(DataHandler& data,
	std::queue<std::unique_ptr<Event>>& events,
	Portfolio& portfolio,
	int short_period,
	int long_period) :
	data(data), events(events), portfolio(portfolio), short_period(short_period), long_period(long_period) {
	symbol_list = data.get_symbols();
	for (const auto& symbol : symbol_list) {
		bars_processed[symbol] = 0;
		short_ema[symbol] = 0.0;
		long_ema[symbol] = 0.0;
	}
}

void MovingAveragesMomentumStrategy::update_ema(const std::string& symbol, double price) {
	bars_processed[symbol]++;

	if (bars_processed[symbol] == 1) {
		short_ema[symbol] = price;
		long_ema[symbol] = price;
	} else {
		double alpha_short = 2.0 / (short_period + 1.0);
		double alpha_long = 2.0 / (long_period + 1.0);

		short_ema[symbol] = (price * alpha_short) + (short_ema[symbol] * (1.0 - alpha_short));
		long_ema[symbol] = (price * alpha_long) + (long_ema[symbol] * (1.0 - alpha_long));
	}
}

void MovingAveragesMomentumStrategy::calculate_signals(const Event& event) {
	if (event.type() != EventType::MARKET)
		return;

	for (const auto& symbol : symbol_list) {
		auto bars = data.get_latest_bars(symbol, 1);
		if (bars.empty())
			continue;

		double price = bars[0].close;
		auto date = bars[0].datetime;

		update_ema(symbol, price);

		if (bars_processed[symbol] < long_period)
			continue;

		double s_ema = short_ema[symbol];
		double l_ema = long_ema[symbol];

		double diff = l_ema - s_ema;
		double factor = std::abs(2.0 * std::atan(diff) / PI);

		if (s_ema >= l_ema) {
			int quantity = static_cast<int>(std::floor(factor * portfolio.get_cash() / price));
			if (quantity != 0) {
				events.push(std::make_unique<SignalEvent>(symbol, date, "LONG", quantity));
			}
		} else {
			int quantity = static_cast<int>(std::floor((factor / 2.0) * portfolio.get_position(symbol)));
			if (quantity != 0) {
				events.push(std::make_unique<SignalEvent>(symbol, date, "SHORT", quantity));
			}
		}
	}
}

void MovingAveragesMomentumStrategy::plot() {}

std::map<std::string, double> MovingAveragesMomentumStrategy::get_indicators() const {
	std::map<std::string, double> indicators;
	std::string symbol = symbol_list[0];

	if (bars_processed.at(symbol) >= short_period) {
		indicators["Short EMA"] = short_ema.at(symbol);
	}
	if (bars_processed.at(symbol) >= long_period) {
		indicators["Long EMA"] = long_ema.at(symbol);
	}

	return indicators;
}
