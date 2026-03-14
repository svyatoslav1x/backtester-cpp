#include "macd.h"
#include <cmath>
#include <numbers>

constexpr double PI = std::numbers::pi;

MovingAveragesLongStrategy::MovingAveragesLongStrategy(DataHandler& data,
	std::queue<std::unique_ptr<Event>>& events,
	Portfolio& portfolio,
	int short_period,
	int long_period) :
	data(data), events(events), portfolio(portfolio), short_period(short_period), long_period(long_period) {
	//std::cout << "this = " << this << std::endl;
	symbol_list = data.get_symbols();
	for (const auto& symbol : symbol_list) {
		/*for (const auto& bar : data.get_latest_bars(symbol, 1)) {
			std::cout << "[DEBUG] Initial bar for " << symbol << ": " << bar.close << " at " << std::chrono::system_clock::to_time_t(bar.datetime) << std::endl;
		}
		std::cout << "[DEBUG] Initializing maps for symbol: " << symbol << std::endl;*/
		bought[symbol] = false;
		bars_processed[symbol] = 0;
		short_ema[symbol] = 0.0;
		long_ema[symbol] = 0.0;
	}
	//std::cout << "this in the end = " << this << std::endl;
	//std::cout << "[DEBUG] Strategy Constructor Finished" << std::endl;
}

std::optional<std::pair<double,double>>
MovingAveragesLongStrategy::update_ema(const std::string& symbol, double price)
{
    bars_processed[symbol]++;

    if (bars_processed[symbol] == 1) {
        short_ema[symbol] = price;
        long_ema[symbol] = price;
        return std::nullopt;
    }

    double short_k = 2.0 / (short_period + 1);
    double long_k  = 2.0 / (long_period + 1);

    short_ema[symbol] = price * short_k + short_ema[symbol] * (1 - short_k);
	long_ema[symbol] = price * long_k +long_ema[symbol] * (1 - long_k);

    if (bars_processed[symbol] < long_period)
        return std::nullopt;

    return std::make_pair(short_ema[symbol], long_ema[symbol]);
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

		auto ema = update_ema(symbol, price);

        if (!ema)
            continue;

		auto [s_ema, l_ema] = *ema;

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

std::optional<std::pair<double,double>>
MovingAveragesLongShortStrategy::update_ema(const std::string& symbol, double price)
{
    bars_processed[symbol]++;

    if (bars_processed[symbol] == 1) {
        short_ema[symbol] = price;
        long_ema[symbol] = price;
        return std::nullopt;
    }

    double short_k = 2.0 / (short_period + 1);
    double long_k  = 2.0 / (long_period + 1);

    short_ema[symbol] = price * short_k + short_ema[symbol] * (1 - short_k);

    long_ema[symbol] = price * long_k +
long_ema[symbol] * (1 - long_k);

    if (bars_processed[symbol] < long_period)
        return std::nullopt;

    return std::make_pair(short_ema[symbol], long_ema[symbol]);
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

		auto ema = update_ema(symbol, price);

        if (!ema)
            continue;

		auto [s_ema, l_ema] = *ema;

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

std::optional<std::pair<double,double>>
MovingAveragesMomentumStrategy::update_ema(const std::string& symbol, double price)
{
    bars_processed[symbol]++;

    if (bars_processed[symbol] == 1) {
        short_ema[symbol] = price;
        long_ema[symbol] = price;
        return std::nullopt;
    }

    double short_k = 2.0 / (short_period + 1);
    double long_k  = 2.0 / (long_period + 1);

    short_ema[symbol] = price * short_k + short_ema[symbol] * (1 - short_k);

    long_ema[symbol] = price * long_k + long_ema[symbol] * (1 - long_k);

    if (bars_processed[symbol] < long_period)
        return std::nullopt;

    return std::make_pair(short_ema[symbol], long_ema[symbol]);
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

		auto ema = update_ema(symbol, price);

        if (!ema)
            continue;

		auto [s_ema, l_ema] = *ema;

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
