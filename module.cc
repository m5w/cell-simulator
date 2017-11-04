#include "module.h"

Module::Module(
    const std::array<CellBuf, number_of_cells> cells_buf,
    const std::array<FloatingPointType, number_of_cells> cells_initial_work,
    const std::array<FloatingPointType, number_of_cells>
        cells_mean_internal_conductance,
    const std::array<FloatingPointType, number_of_cells>
        cells_open_circuit_voltage,
    const FloatingPointType load_current, FloatingPointType &initial_work,
    FloatingPointType &voltage)
    : cells_buf(cells_buf), cells_initial_work(cells_initial_work),
      cells_mean_internal_conductance(cells_mean_internal_conductance),
      cells_open_circuit_voltage(cells_open_circuit_voltage),
      load_current(load_current),
      sum_of_cells_mean_internal_conductance(
          get_sum(cells_mean_internal_conductance)),
      load_work(0.0) {
  const FloatingPointType the_voltage =
      get_voltage(cells_open_circuit_voltage);
  Module::voltage = the_voltage;
  initial_work = get_sum(cells_initial_work);
  voltage = the_voltage;
}

FloatingPointType
Module::get_next_voltage(const FloatingPointType change_in_time) {
  FloatingPointType the_voltage = voltage;
  std::array<FloatingPointType, number_of_cells>
      the_cells_open_circuit_voltage;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    the_cells_open_circuit_voltage[n] =
        cells_buf[n].get_next_open_circuit_voltage(change_in_time,
                                                   the_voltage);

  cells_open_circuit_voltage = the_cells_open_circuit_voltage;
  the_voltage = get_voltage(the_cells_open_circuit_voltage);
  voltage = the_voltage;
  load_work += load_current * the_voltage * change_in_time;
  return the_voltage;
}

FloatingPointType Module::get_sum(
    const std::array<FloatingPointType, number_of_cells> &cells_x) {
  FloatingPointType sum_of_cells_x = 0.0;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    sum_of_cells_x += cells_x[n];

  return sum_of_cells_x;
}
