cmake_minimum_required(VERSION 3.16)
project(backtester-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Sql Network Charts Test)

qt_standard_project_setup()

add_library(backtester_lib
        src/core/backtester.cpp
        src/core/data.cpp
        src/core/event.cpp
        src/core/execution.cpp
        src/core/performance.cpp
        src/core/portfolio.cpp

        src/strategies/macd.cpp
        src/strategies/stop_loss.cpp

        data/database/StrategyDatabase.cpp
        data/database/StrategyDatabase.h

        src/gui/MainWindow.cpp
        src/gui/MainWindow.h
        src/gui/screens/BacktestWindow.cpp
        src/gui/screens/BacktestWindow.h
        src/gui/screens/ChartWidget.cpp
        src/gui/screens/ChartWidget.h
        src/gui/screens/create_strategy_screen.cpp
        src/gui/screens/create_strategy_screen.h
        src/gui/screens/done_screen.cpp
        src/gui/screens/done_screen.h
        src/gui/screens/select_strategy_screen.cpp
        src/gui/screens/select_strategy_screen.h
        src/gui/screens/SimulationEngine.cpp
        src/gui/screens/SimulationEngine.h
        src/gui/screens/start_screen.cpp
        src/gui/screens/start_screen.h
        src/gui/screens/StrategyManager.cpp
        src/gui/screens/StrategyManager.h
        src/gui/screens/style.h
)

target_include_directories(backtester_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/strategies
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/screens
        ${CMAKE_CURRENT_SOURCE_DIR}/data/database
)

target_link_libraries(backtester_lib PUBLIC
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        Qt6::Sql
        Qt6::Network
        Qt6::Charts
)

qt_add_executable(backtester-cpp
        src/main.cpp
)

target_link_libraries(backtester-cpp PRIVATE
        backtester_lib
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(backtester_lib PRIVATE DEBUG)
endif()

if(WIN32)
    set_target_properties(backtester-cpp PROPERTIES WIN32_EXECUTABLE ON)
endif()

if(BUILD_TESTS)
    enable_testing()
   add_subdirectory(tests)
endif()