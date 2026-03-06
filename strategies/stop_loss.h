#pragma once

#include <unordered_map>
#include <string>
#include <queue>
#include <memory>
#include <cmath>

#include "strategy.h"
#include "data_handler.h"
#include "portfolio.h"
#include "event.h"
#include "signal_event.h"

class StopLossStrategy : public Strategy {
private:

    DataHandler& data;
    std::queue<std::unique_ptr<Event>>& events;
    Portfolio& portfolio;

    std::vector<std::string> symbol_list;

    std::unordered_map<std::string, bool> bought;
    std::unordered_map<std::string, double> stop_loss;

    double stop_loss_percentage;

    void initialize_bought() {
        for (const auto& symbol : symbol_list)
            bought[symbol] = false;
    }
    void initialize_stop_loss() {
        for (const auto& symbol : symbol_list)
            stop_loss[symbol] = stop_loss_percentage;
    }

public:

    DataHandler& data_,
        std::queue<std::unique_ptr<Event>>& events_,
        Portfolio& portfolio_,
        double stop_loss_percentage_
        )
        : data(data_),
        events(events_),
        portfolio(portfolio_),
        stop_loss_percentage(stop_loss_percentage_)
        {
            symbol_list = data.get_symbols();

            initialize_bought();
            initialize_stop_loss();
        }

        void calculate_signals(const Event& event) override {
            if (event.type() != EventType::MARKET)
                return;

            for (const auto& symbol : symbol_list) {

                auto data_vec = data.get_latest_data(symbol);

                if (data_vec.empty())
                    continue;

                double latest_close = data_vec.back().close;

                if (!bought[symbol] &&
                    latest_close > stop_loss[symbol] / stop_loss_percentage)
                {
                    int quantity =
                        std::floor(
                            portfolio.get_cash() / latest_close
                        );

                    events.push(
                        std::make_unique<SignalEvent>(
                            symbol,
                            SignalDirection::LONG,
                            quantity
                        )
                    );

                    bought[symbol] = true;

                    stop_loss[symbol] =
                        stop_loss_percentage * latest_close;
                }

                else if (bought[symbol]) {

                    if (latest_close <= stop_loss[symbol]) {

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

                    else {

                        auto last_two =
                            data.get_latest_data(symbol, 2);

                        if (last_two.size() < 2)
                            continue;

                        if (last_two[1].close > last_two[0].close &&
                            stop_loss_percentage * last_two[1].close
                        > stop_loss[symbol])
                        {
                            stop_loss[symbol] =
                                stop_loss_percentage
                                * last_two[1].close;
                        }
                    }
                }
            }
        }

    void plot() override {}
};