#pragma once
#include "event.h"

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual void calculate_signals(const Event& event) = 0;

    virtual void plot() = 0;
};