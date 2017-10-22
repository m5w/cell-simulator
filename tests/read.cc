#include <cstddef>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#include "cell.h"

static inline void set_precision(std::ostream &os);

static FloatingPointType
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
  FloatingPointType load_work = 0.0;
  set_precision(std::cerr);

  while (module_voltage >= 2.5) {
    std::cerr << std::setw(14) << time << std::setw(0) << "  "
              << module_voltage << std::setw(0) << '\n';
    cell_voltage_source_voltage =
        cell.get_next_voltage_source_voltage(change_in_time, module_voltage);
    module_voltage =
        get_module_voltage(load_current, cell_voltage_source_voltage,
                           cell_mean_internal_conductance);
    load_work += load_current * module_voltage;
    ++number_of_changes_in_time;
    time = change_in_time * number_of_changes_in_time;
  }

  const FloatingPointType cell_initial_electric_potential_energy =
      cell.get_initial_electric_potential_energy();
  set_precision(std::cout);
  std::cout << load_work / cell_initial_electric_potential_energy << '\n';
  std::cout << cell_initial_electric_potential_energy -
                   cell.get_electric_potential_energy() - load_work
            << '\n';
}

void set_precision(std::ostream &os) {
  os.precision(std::numeric_limits<FloatingPointType>::max_digits10);
}

FloatingPointType
get_module_voltage(const FloatingPointType load_current,
                   const FloatingPointType cell_voltage_source_voltage,
                   const FloatingPointType cell_mean_internal_conductance) {
  return (cell_voltage_source_voltage * cell_mean_internal_conductance -
          load_current) /
         cell_mean_internal_conductance;
}
