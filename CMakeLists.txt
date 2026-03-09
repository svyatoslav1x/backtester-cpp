# =============================================================================
# Project Configuration
# =============================================================================
cmake_minimum_required(VERSION 3.16)
project(backtester-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# it is used to generate compile_commands.json for clangd
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Find Qt6 Libraries
# =============================================================================
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Create the executable
add_executable(backtester-cpp src/main.cpp)

# =============================================================================
# Source Files
# =============================================================================

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
    # src/qt_app/MainWindow.cpp
    # src/qt_app/DashboardPanel.cpp
    # src/qt_app/ChartingPanel.cpp
    # src/qt_app/BacktesterWorker.cpp
    # src/qt_app/resources/resources.qrc
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
target_link_libraries(backtester-cpp PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

# =============================================================================
# Data Files Management
# =============================================================================
# Copy the 'data' folder to the build directory so the app can find CSVs
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
