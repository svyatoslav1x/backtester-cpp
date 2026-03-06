cmake_minimum_required(VERSION 3.16)
project(backtester-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Sql)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Sql Charts)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
        src/main.cpp
        src/StrategyManager.cpp
        src/StrategyManager.h
        src/ChartWidget.cpp
        src/ChartWidget.h
)

add_executable(backtester-cpp ${SOURCES})

target_link_libraries(backtester-cpp PRIVATE
        Qt6::Widgets
        Qt6::Sql
        Qt6::Charts
)

target_include_directories(backtester-cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)