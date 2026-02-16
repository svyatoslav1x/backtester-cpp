#pragma once
#include <chrono>
#include <string>
#include<vector>
#include "csv_reader.h"
#include "event.h"
#include <unordered_map>

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

    // The last N bars by symbol
    virtual std::vector<Bar> get_latest_bars(
        const std::string& symbol,
        std::size_t N = 1
    ) const = 0;

    // Heartbeat
    virtual void update_bars() = 0;

    // Are there any other data
    virtual bool continue_backtest() const = 0;
};


class HistoricCSVDataHandler : DataHandler {
    // HistoricCSVDataHandler is designed to read CSV files for
    // each requested symbol from disk and provide an interface
    // to obtain the "latest" bar in a manner identical to a live
    // trading interface.
    std::queue<std::unique_ptr<Event>>& events; // events in queue
    std::string csv_dir; //the name of a market
    std::vector<std::string> symbols; //the list of target goods

    std::unordered_map<std::string, std::vector<Bar>> symbol_data; //all data from database
    std::unordered_map<std::string, std::vector<Bar>> latest_symbol_data; // shown data for user

    std::size_t current_index = 0; // track the time from given market
    bool continue_flag = true; // check whether here any other data to control when to finish

    void open_and_parse_csv() {
        for (const auto& symbol : symbols) {
            std::ifstream file(csv_dir + "/" + symbol + ".csv");
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open CSV file for " + symbol);
            }

            CSVreader reader;
            file >> reader;

            // Skip header
            for (std::size_t i = 1; i < reader.data.size(); ++i) {
                const auto& row = reader.data[i];
                if (row.size() < 7) continue;

                Bar bar;
                bar.symbol = symbol;
                bar.datetime = parse_datetime(row[0]);
                bar.open   = std::stod(row[1]);
                bar.high   = std::stod(row[2]);
                bar.low    = std::stod(row[3]);
                bar.close  = std::stod(row[4]);
                bar.volume = std::stod(row[6]);

                symbol_data[symbol].push_back(bar);
            }

            latest_symbol_data[symbol] = {};
        }
    }

    std::chrono::system_clock::time_point parse_datetime(
        const std::string& dt
    ) {
        std::tm tm{};
        std::stringstream ss(dt);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
public:
    HistoricCSVDataHandler(
        std::queue<std::unique_ptr<Event>>& events_,
        const std::string& csv_dir_,
        const std::vector<std::string>& symbols_
    )
        : events(events_), csv_dir(csv_dir_), symbols(symbols_) {
        open_and_parse_csv();
    }

    void update_bars() override {
        for (const auto& symbol : symbols) {
            if (current_index >= symbol_data[symbol].size()) {
                continue_flag = false;
                return;
            }

            latest_symbol_data[symbol].push_back(
                symbol_data[symbol][current_index]
            );
        }

        ++current_index;
        events.push(std::make_unique<MarketEvent>());
    }
    std::vector<Bar> get_latest_bars (const std::string& symbol, std::size_t N) const override {
        const auto& bars = latest_symbol_data.at(symbol);
        if (bars.size() < N) return bars;
        return {bars.end() - N, bars.end()};
    }
    bool continue_backtest() const override {
        return continue_flag;
    }
};
