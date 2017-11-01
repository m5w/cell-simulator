add_executable (test-lerp-cell-voltage test_lerp_cell_voltage.cc)
target_link_libraries (test-lerp-cell-voltage
        ${Boost_LIBRARIES}
        lerp
        )
add_executable (read read.cc)
target_link_libraries (read
        module
        )
