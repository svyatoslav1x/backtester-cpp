cmake_minimum_required(VERSION 3.16)
project(Backtester VERSION 0.1 LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/mingw_64")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

set(SOURCES
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/screens/start_screen.cpp
        src/gui/screens/select_strategy_screen.cpp
        src/gui/screens/create_strategy_screen.cpp
        src/gui/screens/style.h
)

add_executable(Backtester ${SOURCES})

target_link_libraries(Backtester PRIVATE Qt6::Widgets Qt6::Gui)