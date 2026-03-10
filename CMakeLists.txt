cmake_minimum_required(VERSION 3.16)
project(backtester-cpp LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "usr/lib/qt")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# it is used to generate compile_commands.json for clangd
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create the executable
add_executable(backtester-cpp ${SOURCES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# =============================================================================
# Find Qt6 Libraries
# =============================================================================
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql Charts)


# =============================================================================
# Source Files
# =============================================================================
set(SOURCES
        src/main.cpp
        data/database/StrategyDatabase.cpp
)

# --- Backend Core Files ---
target_sources(backtester-cpp PRIVATE
    src/core/backtester.cpp
    src/core/data.cpp
    src/core/event.cpp
    src/core/execution.cpp
    src/core/performance.cpp
    src/core/portfolio.cpp
)

# --- Strategy Implementations ---
target_sources(backtester-cpp PRIVATE
    src/strategies/macd.cpp
    src/strategies/stop_loss.cpp
)

# --- Qt Files (GUI) ---
target_sources(backtester-cpp PRIVATE
    src/gui/screens/StrategyManager.cpp
    src/gui/screens/StrategyManager.h
    src/gui/screens/ChartWidget.cpp
    src/gui/screens/BacktestWindow.cpp
    src/gui/screens/SimulationEngine.cpp
    src/gui/MainWindow.cpp
    src/gui/screens/start_screen.cpp
    src/gui/screens/select_strategy_screen.cpp
    src/gui/screens/create_strategy_screen.cpp
    src/gui/screens/done_screen.cpp
    src/gui/screens/style.h
)

# =============================================================================
# Include Directories
# =============================================================================
target_include_directories(backtester-cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include       # For core headers (event.h, data.h, etc.)
    ${CMAKE_CURRENT_SOURCE_DIR}/src/strategies # For strategy-specific headers (macd.h)
    ${CMAKE_CURRENT_BINARY_DIR}                # For Qt auto-generated headers
)

# =============================================================================
# Link Libraries
# =============================================================================
target_link_libraries(Backtester PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
        Qt6::Charts
)

# =============================================================================
# Platform Specifics
# =============================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(backtester-cpp PRIVATE DEBUG)
endif()

# On Windows, hide the console for GUI apps
if(WIN32)
    set_target_properties(backtester-cpp PROPERTIES WIN32_EXECUTABLE ON)
endif()
