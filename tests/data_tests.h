#pragma once

#include <gtest/gtest.h>
#include <queue>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

#include "data.h"
#include "event.h"

// Тестовый fixture
class DataHandlerTest : public ::testing::Test {
protected:
    std::queue<std::unique_ptr<Event>> events;
    std::vector<std::string> symbols{"TEST"};
    std::string csv_dir = "test_csv";

    std::unique_ptr<HistoricCSVDataHandler> handler;

    // Будет создано временное CSV для теста
    void create_test_csv() {
        std::ofstream file(csv_dir + "/TEST.csv");
        file << "Date,Open,High,Low,Close,Volume\n";
        file << "2026-03-01,10,12,9,11,1000\n";
        file << "2026-03-02,11,13,10,12,1500\n";
        file << "2026-03-03,12,14,11,13,2000\n";
        file.close();
    }

    void SetUp() override {
        // Создаем папку (если нужно)
        std::system(("mkdir " + csv_dir).c_str());
        create_test_csv();
        handler = std::make_unique<HistoricCSVDataHandler>(events, csv_dir, symbols);
    }

    void TearDown() override {
        // Очистка: удаляем файл и папку
        std::remove((csv_dir + "/TEST.csv").c_str());
        std::system(("rmdir " + csv_dir).c_str());
        handler.reset();
    }
};