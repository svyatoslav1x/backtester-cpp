#pragma once
#include <gtest/gtest.h>
#include <queue>
#include <memory>
#include <string>
#include <vector>
#include "../../include/event.h"
#include "../../include/macd.h"

// Mock Bar structure
struct MockBar {
    std::chrono::system_clock::time_point datetime;
    double close;
};

// Mock DataHandler
class MockDataHandler {
public:
    std::vector<std::string> symbols;
    std::unordered_map<std::string, std::vector<MockBar>> bars;

    MockDataHandler() {
        symbols = {"AAPL"};
        bars["AAPL"] = {};
    }

    std::vector<std::string> get_symbols() const { return symbols; }

    std::vector<MockBar> get_latest_bars(const std::string& sym, int N) {
        if (bars.find(sym) != bars.end())
            return bars.at(sym);
        return {};
    }
};

// Mock Portfolio
class MockPortfolio {
public:
    double cash = 1000.0;
    std::unordered_map<std::string, int> positions;

    double get_cash() const { return cash; }
    int get_position(const std::string& sym) const {
        if (positions.find(sym) != positions.end())
            return positions.at(sym);
        return 0;
    }
};