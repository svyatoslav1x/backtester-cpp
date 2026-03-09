#include "../../include/portfolio.h"
#include "../../include/performance.h"

#include <cmath>
#include <iomanip>
#include <sstream>

NaivePortfolio::NaivePortfolio(DataHandler& data,
							   std::queue<std::unique_ptr<Event>>& events,
							   const std::string& strategy_name,
							   double initial_capital) :
	data(data), events(events), strategy_name(strategy_name), initial_capital(initial_capital) {
	symbol_list = data.get_symbols();

	for (const auto& symbol : symbol_list) {
		current_positions[symbol] = 0;
	}

	construct_current_holdings();
}

void NaivePortfolio::construct_current_holdings() {
	for (const auto& symbol : symbol_list) {
		current_holdings.symbols[symbol] = 0.0;
	}
	current_holdings.cash = initial_capital;
	current_holdings.commission = 0.0;
	current_holdings.total = initial_capital;
}

int NaivePortfolio::get_position(const std::string& symbol) const {
	auto it = current_positions.find(symbol);
	if (it != current_positions.end()) {
		return it->second;
	}
	return 0;
}

void NaivePortfolio::update_timeindex(const Event& event) {
	if (event.type() != EventType::MARKET)
		return;

	std::chrono::system_clock::time_point current_time;

	// find the latest valid timepoint
	for (const auto& symbol : symbol_list) {
		auto bars = data.get_latest_bars(symbol, 1);
		if (!bars.empty()) {
			current_time = bars[0].datetime;
			break;
		}
	}

	// updating position record
	PositionRecord pr;
	pr.datetime = current_time;
	for (const auto& symbol : symbol_list) {
		pr.positions[symbol] = current_positions[symbol];
	}
	all_positions.push_back(pr);

	// updating holdings record
	HoldingRecord hr;
	hr.datetime = current_time;
	hr.cash = current_holdings.cash;
	hr.commission = current_holdings.commission;
	hr.total = current_holdings.cash; // Base total is cash

	for (const auto& symbol : symbol_list) {
		auto bars = data.get_latest_bars(symbol, 1);
		double close_price = 0.0;

		if (!bars.empty()) {
			close_price = bars[0].close;
		}

		// Approximate market value = Quantity * Current Price
		double market_value = current_positions[symbol] * close_price;
		hr.symbols[symbol] = market_value;
		hr.total += market_value;
	}

	all_holdings.push_back(hr);
}

void NaivePortfolio::update_positions_from_fill(const FillEvent& fill) {
	int fill_dir = 0;
	if (fill.get_direction() == "BUY") {
		fill_dir = 1;
	} else if (fill.get_direction() == "SELL") {
		fill_dir = -1;
	}

	current_positions[fill.get_symbol()] += fill_dir * fill.get_quantity();
}

void NaivePortfolio::update_holdings_from_fill(const FillEvent& fill) {
	int fill_dir = 0;
	if (fill.get_direction() == "BUY") {
		fill_dir = 1;
	} else if (fill.get_direction() == "SELL") {
		fill_dir = -1;
	}

	// attempt to get the real-world current market price from data
	// to calculate the estimated cost of the trade.
	double fill_cost = fill.get_fill_cost();
	auto bars = data.get_latest_bars(fill.get_symbol(), 1);
	if (!bars.empty()) {
		fill_cost = bars[0].close;
	}

	double cost = fill_cost * fill_dir * fill.get_quantity();

	current_holdings.symbols[fill.get_symbol()] += cost;
	current_holdings.commission += fill.get_commission();
	current_holdings.cash -= (cost + fill.get_commission());
	current_holdings.total -= (cost + fill.get_commission());
}

void NaivePortfolio::update_fill(const Event& event) {
	if (event.type() == EventType::FILL) {
		const auto& fill_event = static_cast<const FillEvent&>(event);
		update_positions_from_fill(fill_event);
		update_holdings_from_fill(fill_event);
	}
}

std::unique_ptr<OrderEvent> NaivePortfolio::generate_naive_order(const SignalEvent& signal) {
	std::string symbol = signal.get_symbol();
	std::string direction = signal.get_direction();
	int quantity = signal.get_quantity();
	int current_quantity = current_positions[symbol];

	if (direction == "LONG") {
		return std::make_unique<OrderEvent>(symbol, "MKT", quantity, "BUY");
	}
	if (direction == "SHORT") {
		return std::make_unique<OrderEvent>(symbol, "MKT", quantity, "SELL");
	}
	if (direction == "EXIT") {
		if (current_quantity > 0) {
			return std::make_unique<OrderEvent>(symbol, "MKT", std::abs(current_quantity), "SELL");
		} else if (current_quantity < 0) {
			return std::make_unique<OrderEvent>(symbol, "MKT", std::abs(current_quantity), "BUY");
		}
	}

	return nullptr;
}

void NaivePortfolio::update_signal(const Event& event) {
	if (event.type() == EventType::SIGNAL) {
		const auto& signal_event = static_cast<const SignalEvent&>(event);
		auto order = generate_naive_order(signal_event);
		if (order != nullptr) {
			events.push(std::move(order));
		}
	}
}

std::vector<std::pair<std::string, std::string>> NaivePortfolio::summary_stats() {
	if (all_holdings.empty()) {
		return {{"Error", "No holdings data recorded."}};
	}

	// construct the equity curve and calculate period returns
	std::vector<double> equity_curve;
	std::vector<double> period_returns;
	equity_curve.reserve(all_holdings.size());
	period_returns.reserve(all_holdings.size());

	double prev_total = initial_capital;
	for (const auto& hr : all_holdings) {
		equity_curve.push_back(hr.total);

		// Return = (Total_t - Total_{t-1}) / Total_{t-1}
		double current_return = (hr.total - prev_total) / prev_total;
		period_returns.push_back(current_return);

		prev_total = hr.total;
	}

	double sharpe_ratio = performance::calculate_sharpe_ratio(period_returns, 252);

	auto [max_dd, dd_duration] = performance::calculate_drawdowns(equity_curve);

	double total_return_pct = ((equity_curve.back() / initial_capital) - 1.0) * 100.0;
	double max_dd_pct = (max_dd / initial_capital) * 100.0; // estimate percentage format

	std::ostringstream tr_ss, sr_ss, dd_ss, dur_ss;
	tr_ss << std::fixed << std::setprecision(2) << total_return_pct << "%";
	sr_ss << std::fixed << std::setprecision(2) << sharpe_ratio;
	dd_ss << std::fixed << std::setprecision(2) << max_dd_pct << "%";
	dur_ss << dd_duration;

	return {{"Total Return", tr_ss.str()},
			{"Sharpe Ratio", sr_ss.str()},
			{"Max Drawdown", dd_ss.str()},
			{"Drawdown Duration", dur_ss.str()}};
}

void NaivePortfolio::plot_performance() {}
