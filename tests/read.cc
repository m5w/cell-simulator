#include <cstddef>

#include <fstream>
#include <iostream>
#include <limits>

#include "cell.h"

FloatingPointType
get_module_voltage(const FloatingPointType load_current,
                   const FloatingPointType cell_voltage_source_voltage,
                   const FloatingPointType cell_mean_internal_conductance);

int main() {
  std::ifstream is("../test-liion-06.bin");
  is.exceptions(std::ios_base::badbit | std::ios_base::failbit |
                std::ios_base::eofbit);
  FloatingPointType cell_mean_internal_conductance;
  FloatingPointType cell_voltage_source_voltage;
  Cell cell(is, cell_mean_internal_conductance, cell_voltage_source_voltage);
  constexpr FloatingPointType load_current = 5.0;
  FloatingPointType module_voltage =
      get_module_voltage(load_current, cell_voltage_source_voltage,
                         cell_mean_internal_conductance);
  FloatingPointType time = 0.0;
  constexpr FloatingPointType change_in_time = 1.0;
  std::size_t number_of_changes_in_time = 0;
  std::cout.precision(std::numeric_limits<FloatingPointType>::max_digits10);

  while (module_voltage >= 2.5) {
    std::cout << time << ", " << module_voltage << '\n';
    cell_voltage_source_voltage =
        cell.get_next_voltage_source_voltage(change_in_time, module_voltage);
    module_voltage =
        get_module_voltage(load_current, cell_voltage_source_voltage,
                           cell_mean_internal_conductance);
    ++number_of_changes_in_time;
    time = change_in_time * number_of_changes_in_time;
  }

  std::cout << cell.get_electric_potential_energy() << '\n';
}

FloatingPointType
get_module_voltage(const FloatingPointType load_current,
                   const FloatingPointType cell_voltage_source_voltage,
                   const FloatingPointType cell_mean_internal_conductance) {
  return (cell_voltage_source_voltage * cell_mean_internal_conductance -
          load_current) /
         cell_mean_internal_conductance;
}
