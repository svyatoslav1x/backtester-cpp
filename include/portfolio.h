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

	// handles new trading signals from a strategy
	virtual void update_signal(const Event& event) = 0;
	// handles fill events received from an execution handler
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
	// A position is simply the quantity of the asset.
	// Negative positions mean the asset has been shorted.
	//
	// Holdings decribe the current maket value of the positions held.
	// Where "the current market value" is the closing price of the
	// current market bar.

	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	std::vector<std::string> symbol_list;

	std::string strategy_name;
	double initial_capital{100000}; // 100,000 USD

	// contains current positions for the last market bar update
	std::unordered_map<std::string, int> current_positions;

	// stores the most up to date holdings for each symbol
	struct {
		double cash;
		double commission;
		double total; // cash + (positions * current market price)
		std::unordered_map<std::string, double> symbols;
	} current_holdings;

	// stores a list of all previous positions recorded at the timestamp
	// of a market data event
	std::vector<PositionRecord> all_positions;
	// stores a list of all previous holdings recorded at the timestamp
	// of a market data event
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
