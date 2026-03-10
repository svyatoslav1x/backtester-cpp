#include "../../include/data.h"
#include "../../include/csv_reader.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

HistoricCSVDataHandler::HistoricCSVDataHandler(std::queue<std::unique_ptr<Event>>& events_,
											   const std::string& csv_dir_,
											   const std::vector<std::string>& symbol_list_) :
	events(events_), csv_dir(csv_dir_), symbol_list(symbol_list_) {
	open_and_parse_csv();

	for (const auto& symbol : symbol_list) {
		latest_symbol_data[symbol] = {}; // default initialization of vector of bars for each symbol
	}
}

double HistoricCSVDataHandler::safe_stod(const std::string& s) const {
	if (s.empty())
		return 0.0;
	try {
		return std::stod(s);
	} catch (...) {
		return 0.0;
	}
}

std::chrono::system_clock::time_point HistoricCSVDataHandler::parse_datetime(const std::string& dt) const {
	std::tm tm{};
	std::stringstream ss(dt);
	ss >> std::get_time(&tm, "%Y-%m-%d");
	return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

void HistoricCSVDataHandler::open_and_parse_csv() {
	for (const auto& symbol : symbol_list) {
		std::ifstream file(csv_dir + "/" + symbol + ".csv");
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open CSV file for " + symbol);
		}

		CSVreader reader;
		file >> reader;
		const auto& data = reader.get_data();

		if (data.empty())
			continue;

		// dynamically map headers to handle YAHOO vs NASDAQ differences
		int date_idx = -1, open_idx = -1, high_idx = -1, low_idx = -1, close_idx = -1, vol_idx = -1;
		const auto& header = data[0];

		for (std::size_t j = 0; j < header.size(); ++j) {
			const std::string& col = header[j];
			if (col == "Date")
				date_idx = static_cast<int>(j);
			else if (col == "Open")
				open_idx = static_cast<int>(j);
			else if (col == "High" || col == "High price")
				high_idx = static_cast<int>(j);
			else if (col == "Low" || col == "Low price")
				low_idx = static_cast<int>(j);
			else if (col == "Close" || col == "Closing price")
				close_idx = static_cast<int>(j);
			else if (col == "Volume" || col == "Total volume")
				vol_idx = static_cast<int>(j);
		}

		if (date_idx == -1 || close_idx == -1) {
			throw std::runtime_error("CSV missing 'Date' or 'Close' column for " + symbol);
		}

		// parsing rows
		for (std::size_t i = 1; i < data.size(); ++i) {
			const auto& row = data[i];
			if (row.size() <= static_cast<std::size_t>(close_idx) || row[date_idx].empty())
				continue;

			Bar bar;
			bar.datetime = parse_datetime(row[date_idx]);
			bar.close = safe_stod(row[close_idx]);
			bar.open = (open_idx != -1 && row.size() > open_idx) ? safe_stod(row[open_idx]) : bar.close;
			bar.high = (high_idx != -1 && row.size() > high_idx) ? safe_stod(row[high_idx]) : bar.close;
			bar.low = (low_idx != -1 && row.size() > low_idx) ? safe_stod(row[low_idx]) : bar.close;
			bar.volume = (vol_idx != -1 && row.size() > vol_idx) ? safe_stod(row[vol_idx]) : 0.0;

			symbol_data[symbol].push_back(bar);
		}

		// ensure chronological order
		if (symbol_data[symbol].size() > 1) {
			if (symbol_data[symbol].front().datetime > symbol_data[symbol].back().datetime) {
				std::reverse(symbol_data[symbol].begin(), symbol_data[symbol].end());
			}
		}
	}
}

void HistoricCSVDataHandler::update_bars() {
	// Pushes one new bar from 'symbol_data' into
	// 'latest_symbol_data' for each symbol
	// As program progresses, 'latest_symbol_data'
	// copies all bars from 'symbol_data' for each symbol
	//
	// This where MarketEvents are generated and pushed into the event queue
	for (const auto& symbol : symbol_list) {
		if (current_index >= symbol_data[symbol].size()) {
			continue_backtest = false;
			return;
		}

		latest_symbol_data[symbol].push_back(symbol_data[symbol][current_index]);
	}

	++current_index;
	events.push(std::make_unique<MarketEvent>());
}

std::vector<Bar> HistoricCSVDataHandler::get_latest_bars(const std::string& symbol, std::size_t N) const {
	const auto& bars = latest_symbol_data.at(symbol);
	if (bars.size() < N)
		return bars;
	return {bars.end() - N, bars.end()};
}

bool HistoricCSVDataHandler::if_continue_backtest() const { return continue_backtest; }

std::vector<std::string> HistoricCSVDataHandler::get_symbols() const { return symbol_list; }
