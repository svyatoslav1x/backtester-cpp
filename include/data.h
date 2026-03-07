#pragma once

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "csv_reader.h"
#include "event.h"

struct Bar {
	std::chrono::system_clock::time_point datetime;
	double open;
	double high;
	double low;
	double close;
	double volume;
};

class DataHandler {
public:
	virtual ~DataHandler() = default;

	// Returns the last N bars by symbol which has been proccessed
	// BY the current heartbeat timestamp
	virtual std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const = 0;

	// This where MarketEvents are generated and pushed into the event queue
	virtual void update_bars() = 0;

	// Are there any data in CSV files left to process?
	virtual bool if_continue_backtest() const = 0;

	// Returns the list of symbols being tracked
	virtual std::vector<std::string> get_symbols() const = 0;
};

// Handles historical CSV data for equities in an
// Open-Low-High-Close-Volume-OpenInterest set of bars
class HistoricCSVDataHandler : public DataHandler {
	// HistoricCSVDataHandler is designed to read CSV files for
	// each requested symbol from 'symbol_list' and provide an interface
	// to obtain the "latest" bar in a manner identical to a live
	// trading interface.
	//
	// It will be assumed that all CSV files names are of the form
	// 'symbol.csv', where 'symbol' is a string from the symbol_list.
	//
	// It will be assumed that each CSV file is sorted by date
	// in ascending order.

	std::queue<std::unique_ptr<Event>>& events; // the event queue of the backtester
	std::string csv_dir; // the absolute directory path of the CSV files (without trailing slash '/')
	std::vector<std::string> symbol_list; // the list of target goods

	// all data from the CSV files for each symbol
	std::unordered_map<std::string, std::vector<Bar>> symbol_data;
	std::unordered_map<std::string, std::vector<Bar>> latest_symbol_data; // shown data for user

	// 'current_index' is used to track the current position in the symbol_data for each symbol
	// meaning the number of lines in each CSV file should be the same!!!
	std::size_t current_index = 0; // track the time
	bool continue_backtest = true; // check whether here any other data to control when to finish

	// Helper to safely convert strings with missing data
	double safe_stod(const std::string& s) const;

	// system_clock - истемные часы
	// time_point - конкретная точка во времени
	// time_t - работает конкретно со временем из mktime и есть возможность измерить duration
	std::chrono::system_clock::time_point parse_datetime(const std::string& dt) const;

	void open_and_parse_csv();
public:
	HistoricCSVDataHandler(std::queue<std::unique_ptr<Event>>& events_,
						   const std::string& csv_dir_,
						   const std::vector<std::string>& symbol_list_);

	void update_bars() override;
	std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N) const override;
	bool if_continue_backtest() const override;
	std::vector<std::string> get_symbols() const override;
};
