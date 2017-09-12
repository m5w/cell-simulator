cmake_minimum_required (VERSION 3.1)
project (BatterySimulator)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)
add_library (cell
        error.c
        interpolate_cell_voltage.c
        )
