#pragma once

#include <chrono>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "data.h"
#include "event.h"

class Portfolio {
public:
	virtual ~Portfolio() = default;

	virtual void update_timeindex(const Event& event) = 0;
	virtual void update_signal(const Event& event) = 0;
	virtual void update_fill(const Event& event) = 0;

	// Getters for strategies
	virtual double get_cash() const = 0;
	virtual int get_position(const std::string& symbol) const = 0;
};

struct PositionRecord {
	std::chrono::system_clock::time_point datetime;
	std::unordered_map<std::string, int> positions;
};

struct HoldingRecord {
	std::chrono::system_clock::time_point datetime;
	double cash;
	double commission;
	double total;
	std::unordered_map<std::string, double> symbols;
};

class NaivePortfolio : public Portfolio {
private:
	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	std::vector<std::string> symbol_list;

	std::string strategy_name;
	double initial_capital;

	// current state
	std::unordered_map<std::string, int> current_positions;

	struct {
		double cash;
		double commission;
		double total;
		std::unordered_map<std::string, double> symbols;
	} current_holdings;

	// historical state
	std::vector<PositionRecord> all_positions;
	std::vector<HoldingRecord> all_holdings;

	// helpers
	void construct_current_holdings();
	void update_positions_from_fill(const FillEvent& fill);
	void update_holdings_from_fill(const FillEvent& fill);
	std::unique_ptr<OrderEvent> generate_naive_order(const SignalEvent& signal);
public:
	NaivePortfolio(DataHandler& data,
				   std::queue<std::unique_ptr<Event>>& events,
				   const std::string& strategy_name,
				   double initial_capital = 100000.0);

	// core event handlers
	void update_timeindex(const Event& event) override;
	void update_signal(const Event& event) override;
	void update_fill(const Event& event) override;

	// strategy API
	double get_cash() const override { return current_holdings.cash; }
	int get_position(const std::string& symbol) const override;

	// analytics
	std::vector<std::pair<std::string, std::string>> summary_stats();
	void plot_performance();
};
