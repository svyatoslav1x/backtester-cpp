#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <cmath>

#include "strategy.h"
#include "event.h"
#include "signal_event.h"
#include "data_handler.h"
#include "portfolio.h"

class MovingAveragesLongStrategy : public Strategy
{
private:

    DataHandler& data;
    std::queue<std::unique_ptr<Event>>& events;
    Portfolio& portfolio;

    std::vector<std::string> symbol_list;

    int short_period;
    int long_period;

    std::unordered_map<std::string, bool> bought;

public:

    MovingAveragesLongStrategy(
        DataHandler& data_,
        std::queue<std::unique_ptr<Event>>& events_,
        Portfolio& portfolio_,
        int short_period_,
        int long_period_
    )
        :
        data(data_),
        events(events_),
        portfolio(portfolio_),
        short_period(short_period_),
        long_period(long_period_)
    {
        symbol_list = data.get_symbols();

        for (const auto& s : symbol_list)
            bought[s] = false;
    }

private:

    double calculate_ema(const std::vector<double>& prices, int period)
    {
        if (prices.size() < period)
            return 0.0;

        double alpha = 2.0 / (period + 1.0);

        double ema = prices[prices.size() - period];

        for (size_t i = prices.size() - period + 1; i < prices.size(); ++i)
        {
            ema = alpha * prices[i] + (1 - alpha) * ema;
        }

        return ema;
    }

public:

    void calculate_signals(const Event& event) override
    {
        if (event.type() != EventType::MARKET)
            return;

        for (const auto& symbol : symbol_list)
        {
            auto bars = data.get_latest_data(symbol);

            if (bars.size() < long_period)
                continue;

            std::vector<double> prices;

            for (const auto& bar : bars)
                prices.push_back(bar.close);

            double short_ema = calculate_ema(prices, short_period);
            double long_ema = calculate_ema(prices, long_period);

            double price = prices.back();

            if (!bought[symbol] && short_ema > long_ema)
            {
                int quantity =
                    std::floor(portfolio.get_cash() / price);

                events.push(
                    std::make_unique<SignalEvent>(
                        symbol,
                        SignalDirection::LONG,
                        quantity
                    )
                );

                bought[symbol] = true;
            }

            else if (bought[symbol] && short_ema < long_ema)
            {
                int quantity =
                    portfolio.get_position(symbol);

                events.push(
                    std::make_unique<SignalEvent>(
                        symbol,
                        SignalDirection::EXIT,
                        quantity
                    )
                );

                bought[symbol] = false;
            }
        }
    }

    void plot() override {}
};