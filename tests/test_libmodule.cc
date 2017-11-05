#define BOOST_TEST_MODULE test_libmodule
#include <boost/test/included/unit_test.hpp>

#include <cstddef>

#include <iomanip>
#include <iostream>
#include <limits>

#include "floating_point_type.h"

#include "cells.h"
#include "module.h"

static inline void set_precision(std::ostream &os);

BOOST_AUTO_TEST_CASE(test_Module) {
  const Cells the_cells("../cells.bin");
  FloatingPointType module_initial_work;
  FloatingPointType module_voltage;
  Module the_module =
      the_cells.create_module({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 5.0,
                              module_initial_work, module_voltage);
  FloatingPointType time = 0.0;
  std::size_t number_of_changes_in_time = 0;
  constexpr FloatingPointType change_in_time = 1.0;
  set_precision(std::cerr);

  while (module_voltage >= 2.5) {
    std::cerr << std::setw(14) << time << std::setw(0) << "  "
              << module_voltage << '\n';
    module_voltage = the_module.get_next_voltage(change_in_time);
    ++number_of_changes_in_time;
    time = number_of_changes_in_time * change_in_time;
  }

  std::cerr << '\n';
  set_precision(std::cout);
  const FloatingPointType module_load_work = the_module.get_load_work();
  std::cout << module_load_work / module_initial_work << '\n';
  std::cout << module_initial_work - the_module.get_work() - module_load_work
            << '\n';
}

void set_precision(std::ostream &os) {
  os.precision(std::numeric_limits<FloatingPointType>::max_digits10);
}
