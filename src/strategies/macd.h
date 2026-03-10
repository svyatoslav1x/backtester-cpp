#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../include/data.h"
#include "../../include/event.h"
#include "../../include/portfolio.h"
#include "../../include/strategy.h"

// Long Only
class MovingAveragesLongStrategy : public Strategy {
private:
	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	Portfolio& portfolio;
	std::vector<std::string> symbol_list;

	int short_period;
	int long_period;

	std::unordered_map<std::string, bool> bought;
	std::unordered_map<std::string, int> bars_processed;
	std::unordered_map<std::string, double> short_ema;
	std::unordered_map<std::string, double> long_ema;

	void update_ema(const std::string& symbol, double price);
public:
	MovingAveragesLongStrategy(DataHandler& data,
							   std::queue<std::unique_ptr<Event>>& events,
							   Portfolio& portfolio,
							   int short_period,
							   int long_period);

	void calculate_signals(const Event& event) override;
	void plot() override;
	std::map<std::string, double> get_indicators() const override;
	double get_short_ema(const std::string& sym) { return short_ema[sym]; }
	double get_long_ema(const std::string& sym) { return long_ema[sym]; }
};

// Long and Short
class MovingAveragesLongShortStrategy : public Strategy {
private:
	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	Portfolio& portfolio;
	std::vector<std::string> symbol_list;

	int short_period;
	int long_period;

	std::unordered_map<std::string, bool> bought;
	std::unordered_map<std::string, int> bars_processed;
	std::unordered_map<std::string, double> short_ema;
	std::unordered_map<std::string, double> long_ema;

	void update_ema(const std::string& symbol, double price);
public:
	MovingAveragesLongShortStrategy(DataHandler& data,
									std::queue<std::unique_ptr<Event>>& events,
									Portfolio& portfolio,
									int short_period,
									int long_period);

	void calculate_signals(const Event& event) override;
	void plot() override;
};

// Momentum Position Sizing
class MovingAveragesMomentumStrategy : public Strategy {
private:
	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	Portfolio& portfolio;
	std::vector<std::string> symbol_list;

	int short_period;
	int long_period;

	std::unordered_map<std::string, int> bars_processed;
	std::unordered_map<std::string, double> short_ema;
	std::unordered_map<std::string, double> long_ema;

	void update_ema(const std::string& symbol, double price);
public:
	MovingAveragesMomentumStrategy(DataHandler& data,
								   std::queue<std::unique_ptr<Event>>& events,
								   Portfolio& portfolio,
								   int short_period,
								   int long_period);

	void calculate_signals(const Event& event) override;
	void plot() override;
};
