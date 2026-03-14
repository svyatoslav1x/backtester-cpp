include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP NEW
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

qt_add_executable(backtester_tests
        run_tests.cpp
        start_screen_tests.cpp
        done_screen_tests.cpp
        select_strategy_screen_tests.cpp
        create_strategy_screen_tests.cpp
        strategy_database_tests.cpp
        mainwindow_tests.cpp
        strategy_manager_tests.cpp
        chart_widget_tests.cpp
        backtest_window_tests.cpp
        simulation_engine_tests.cpp

        data_tests.cpp
        event_tests.cpp
        macd_tests.cpp
        stop_loss_tests.cpp
        portfolio_test.cpp
        execution_tests.cpp
)

target_include_directories(backtester_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/strategies
        ${CMAKE_SOURCE_DIR}/src/gui
        ${CMAKE_SOURCE_DIR}/src/gui/screens
        ${CMAKE_SOURCE_DIR}/data/database
)

target_link_libraries(backtester_tests PRIVATE
        backtester_lib
        Qt6::Test
        GTest::gtest
)

add_test(NAME backtester_tests COMMAND backtester_tests)
