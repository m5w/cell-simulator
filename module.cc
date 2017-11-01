#include "module.h"

Module::Module(
    const std::array<CellBuf, number_of_cells> cells_buf,
    const std::array<FloatingPointType, number_of_cells> cells_initial_work,
    const std::array<FloatingPointType, number_of_cells>
        cells_mean_internal_conductance,
    const std::array<FloatingPointType, number_of_cells>
        cells_open_circuit_voltage,
    const FloatingPointType load_current, FloatingPointType &voltage)
    : cells_buf(cells_buf), cells_initial_work(cells_initial_work),
      cells_mean_internal_conductance(cells_mean_internal_conductance),
      cells_open_circuit_voltage(cells_open_circuit_voltage),
      load_current(load_current),
      sum_of_cells_mean_internal_conductance(
          get_sum_of_cells_mean_internal_conductance(
              cells_mean_internal_conductance)),
      load_work(0.0) {
  const FloatingPointType the_voltage = get_voltage();
  Module::voltage = the_voltage;
  voltage = the_voltage;
}

FloatingPointType
Module::get_next_voltage(const FloatingPointType change_in_time) {
  FloatingPointType the_voltage = voltage;
  std::array<FloatingPointType, number_of_cells> the_cell_open_circuit_voltage;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    the_cell_open_circuit_voltage[n] =
        cells_buf[n].get_next_open_circuit_voltage(change_in_time,
                                                   the_voltage);

  the_voltage = get_voltage();
  voltage = the_voltage;
  load_work += load_current * the_voltage * change_in_time;
  return the_voltage;
}

FloatingPointType Module::get_sum_of_cells_mean_internal_conductance(
    const std::array<FloatingPointType, number_of_cells>
        &cells_mean_internal_conductance) {
  FloatingPointType the_sum_of_cells_mean_internal_conductance = 0.0;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    the_sum_of_cells_mean_internal_conductance +=
        cells_mean_internal_conductance[n];

  return the_sum_of_cells_mean_internal_conductance;
}

FloatingPointType Module::get_voltage() {
  // \sum_{n = 1}^\textrm{number of cells} v_n G_n
  //
  // A cell's current is
  //
  //   (v_n - v) G_n ,
  //
  // where v is the cell's open-circuit voltage,
  //       v is the module voltage, and
  //     G_n is the cell's mean internal conductance.
  //
  // The module voltage is the voltage of the node that connects all the cells'
  // internal resistors.  Suppose that v were 0 V; then the cell's current
  // would be
  //
  //   v_n G_n .
  //
  // Since the ground node connects all the cells' ideal voltage sources at
  // their negative terminals, for the module voltage to be 0 V would be for
  // the module to be connected to a short circuit.
  FloatingPointType sum_of_cells_short_circuit_current = 0.0;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    sum_of_cells_short_circuit_current +=
        cells_open_circuit_voltage[n] * cells_mean_internal_conductance[n];

  return (sum_of_cells_short_circuit_current - load_current) /
         sum_of_cells_mean_internal_conductance;
}
