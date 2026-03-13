#pragma once
#include <gtest/gtest.h>
#include <chrono>
#include "../../include/event.h"

class EventTest : public ::testing::Test {
protected:
    std::chrono::system_clock::time_point now;

    void SetUp() override {
        now = std::chrono::system_clock::now();
    }

    void TearDown() override {
        // Можно очищать ресурсы, если нужно
    }
};