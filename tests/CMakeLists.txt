add_executable (test-interpolate-cell-voltage test_interpolate_cell_voltage.cc)
target_link_libraries (test-interpolate-cell-voltage ${Boost_LIBRARIES} cell)
