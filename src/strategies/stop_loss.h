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

class StopLossStrategy : public Strategy {
private:
	DataHandler& data;
	std::queue<std::unique_ptr<Event>>& events;
	Portfolio& portfolio;

	std::vector<std::string> symbol_list;
	std::unordered_map<std::string, bool> bought;
	std::unordered_map<std::string, double> stop_loss;

	double stop_loss_percentage;
public:
	StopLossStrategy(DataHandler& data,
					 std::queue<std::unique_ptr<Event>>& events,
					 Portfolio& portfolio,
					 double stop_loss_percentage);

	void calculate_signals(const Event& event) override;
	void plot() override;
};
