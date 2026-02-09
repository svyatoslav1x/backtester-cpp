# =============================================================================
# Project Configuration
# =============================================================================
cmake_minimum_required(VERSION 3.16)
project(backtester-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# Find Qt6 Libraries
# =============================================================================
# This command finds the necessary Qt6 components.
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Automatically handle Qt's special build steps (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Name of the final application executable
add_executable(backtester-cpp "")

# =============================================================================
# Source Files
# =============================================================================

# --- Backend Files ---
target_sources(backtester-cpp PRIVATE
    # src/core/HistoricCSVDataHandler.cpp
    # src/core/Portfolio.cpp
    # src/core/Execution.cpp
)

# --- Strategy Files ---
target_sources(backtester-cpp PRIVATE
    # src/strategy/MovingAveragesLongStrategy.cpp
)

# --- Qt Files (GUI) ---
target_sources(backtester-cpp PRIVATE
    src/main.cpp
    # src/qt_app/MainWindow.cpp
    # src/qt_app/DashboardPanel.cpp
    # src/qt_app/ChartingPanel.cpp
    # src/qt_app/BacktesterWorker.cpp
    # src/qt_app/resources/resources.qrc # Qt resource file
)

# This doesn't affect compilation but improves the developer experience.
target_sources(backtester-cpp PRIVATE
    # src/core/Events.hpp
    # src/core/ThreadSafeQueue.hpp
    # src/core/HistoricCSVDataHandler.hpp
    # src/core/Portfolio.hpp
    # src/core/Execution.hpp
    # src/strategy/Strategy.hpp
    # src/strategy/MovingAveragesLongStrategy.hpp
    # src/qt_app/MainWindow.hpp
    # src/qt_app/DashboardPanel.hpp
    # src/qt_app/ChartingPanel.hpp
    # src/qt_app/BacktesterWorker.hpp
)

# =============================================================================
# Link Libraries
# =============================================================================
# This connects application to the Qt libraries it needs.
target_link_libraries(backtester-cpp PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

# =============================================================================
# Include Directories
# =============================================================================
# Tell the compiler where to find header files
target_include_directories(backtester-cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR} # Needed for Qt's auto-generated headers
)

# =============================================================================
# Misc
# =============================================================================
# Define a preprocessor macro for easier debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(backtester-cpp PRIVATE DEBUG)
endif()

# For Windows, create a GUI application instead of a console one
if(WIN32)
    set_target_properties(backtester-cpp PROPERTIES WIN32_EXECUTABLE ON)
endif()
