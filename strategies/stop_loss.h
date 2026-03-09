#pragma once

#include <unordered_map>
#include <string>
#include <queue>
#include <memory>
#include <cmath>

#include "strategy.h"
#include "include/data.h"
#include "include/portfolio.h"
#include "include/event.h"
#include "include/event.h"

//Стоп - лосс(англ.stop loss) — это отложенный приказ брокеру закрыть сделку с убытком на определённом уровне цены.
// Цель — ограничить убытки, если рынок разворачивается против позиции.


class StopLossStrategy : public Strategy {
    DataHandler& data; // market data
    std::queue<std::unique_ptr<Event>>& events; //events queue
    Portfolio& portfolio;
    std::vector<std::string> symbol_list; //the list of trading objects
    std::unordered_map<std::string, double> stop_loss; //the level of stop-loss for each object
    double stop_loss_percentage; // when to sell

    void initialize_stop_loss() {
        for (const auto& symbol : symbol_list)
            stop_loss[symbol] = 0.0;
    }
public:

    StopLossStrategy(
        DataHandler& data_,
        std::queue<std::unique_ptr<Event>>& events_,
        Portfolio& portfolio_,
        const std::vector<std::string>& symbols_,
        double stop_loss_percentage_
    )
        :
        data(data_),
        events(events_),
        portfolio(portfolio_),
        symbol_list(symbols_),
        stop_loss_percentage(stop_loss_percentage_)
    {
        initialize_stop_loss();
    }

    void calculate_signals(const Event& event) override {
        if (event.type() != EventType::MARKET)
            return;

        for (const auto& symbol : symbol_list) {

            auto bars = data.get_latest_bars(symbol, 2);

            if (bars.empty())
                continue;
            double latest_close = bars.back().close;

            // to open LONG
            if (!portfolio.get_position(symbol) &&
                latest_close > stop_loss[symbol] / stop_loss_percentage)
            {
                int quantity =
                    std::floor(
                        portfolio.get_cash() / latest_close
                    ); // how much to buy

                events.push(
                    std::make_unique<SignalEvent>(
                        symbol,
                        bars[bars.size() - 1].datetime,
                        "LONG",
                        quantity
                    ) // add an event to queue
                );

                stop_loss[symbol] =
                    stop_loss_percentage * latest_close; // new stop_loss
            }
            //if there is already a position
            else if (portfolio.get_position(symbol) > 0) {
                //to close it
                if (latest_close <= stop_loss[symbol]) {

                    int quantity =
                        portfolio.get_position(symbol);

                    events.push(
                        std::make_unique<SignalEvent>(
                            symbol,
                            bars[bars.size() - 1].datetime,
                            "EXIT",
                            quantity));
                }
                else {
                    auto last_two = data.get_latest_bars(symbol, 2);
                    if (bars.size() < 2)
                        continue;
                    if (bars[1].close > bars[0].close &&
                        stop_loss_percentage * last_two[1].close
                        > stop_loss[symbol]) // if the price is rising and new stop loss is higher -> need to update stop_loss
                    {
                        stop_loss[symbol] =
                            stop_loss_percentage
                            * bars[1].close;
                    }
                }
            }
        }
    }

    void plot() override {}
};