cmake_minimum_required (VERSION 3.1)
project (BatterySimulator)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${PROJECT_SOURCE_DIR})
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 14)
add_library (lerp
        lerp.c
        )
add_library (cell
        cell_discharge_curve.cc
        )
target_link_libraries (cell lerp)
add_subdirectory (tests)
