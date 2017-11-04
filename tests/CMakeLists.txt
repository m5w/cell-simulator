add_executable (test-liblerp test_liblerp.cc)
target_link_libraries (test-liblerp
        ${Boost_LIBRARIES}
        lerp
        )
add_executable (test-libmodule test_libmodule.cc)
target_link_libraries (test-libmodule
        module
        )
