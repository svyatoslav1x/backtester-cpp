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
    std::queue<std::unique_ptr<Event>>& events;
    std::string csv_dir; //Absolute directory path to the CSV files.
    std::vector<std::string> symbols;

    // All data
    std::unordered_map<std::string, std::vector<Bar>> symbol_data;

    // Already shown bars
    std::unordered_map<std::string, std::vector<Bar>> latest_symbol_data;

    std::size_t current_index = 0;
    bool continue_flag = true;
};
