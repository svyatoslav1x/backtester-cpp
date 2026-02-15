#include <chrono>
#include <iostream>
#include <string>

class DataHandler {
    //Pushes the latest bar to the latest symbol structure for all symbols in the symbol list. - обновляет данные до следующего бара
    virtual void update_bars();

    // Есть ли ещё данные
    virtual bool continue_backtest() const;

    // Returns the last N bars from the latest_symbol list, or fewer if less bars are available.
    virtual double get_latest_close(const std::string& symbol) const;
};

class HistoricCSVDataHandler : DataHandler {
    // HistoricCSVDataHandler is designed to read CSV files for
    // each requested symbol from disk and provide an interface
    // to obtain the "latest" bar in a manner identical to a live
    // trading interface.

};
