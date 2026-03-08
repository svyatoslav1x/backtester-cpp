cmake_minimum_required(VERSION 3.16)
project(Backtester VERSION 0.1 LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/mingw_64")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql)

set(SOURCES
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/screens/start_screen.cpp
        src/gui/screens/select_strategy_screen.cpp
        src/gui/screens/create_strategy_screen.cpp
        src/gui/screens/done_screen.cpp
        src/gui/screens/style.h
        data/database/StrategyDatabase.cpp
)

add_executable(Backtester ${SOURCES})

target_link_libraries(Backtester PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
)