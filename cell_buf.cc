#include "cell_buf.h"

#include "cell.h"

CellBuf::CellBuf(const Cell *const the_cell_pointer,
                 CellPointsBuf the_cell_points_buf,
                 FloatingPointType &mean_internal_conductance,
                 FloatingPointType &open_circuit_voltage)
    : the_cell_points_buf(the_cell_points_buf) {
  const FloatingPointType the_mean_internal_conductance =
      the_cell_pointer->mean_internal_conductance;
  CellBuf::mean_internal_conductance = the_mean_internal_conductance;
  mean_internal_conductance = the_mean_internal_conductance;
  constexpr FloatingPointType the_charge_supplied = 0.0;
  const FloatingPointType the_open_circuit_voltage =
      get_open_circuit_voltage(the_charge_supplied);
  charge_supplied = the_charge_supplied;
  CellBuf::open_circuit_voltage = the_open_circuit_voltage;
  open_circuit_voltage = the_open_circuit_voltage;
}

FloatingPointType CellBuf::get_next_open_circuit_voltage(
    const FloatingPointType change_in_time,
    const FloatingPointType module_voltage) {
  const FloatingPointType the_charge_supplied =
      charge_supplied +
      (open_circuit_voltage - module_voltage) * mean_internal_conductance *
          change_in_time;
  const FloatingPointType the_open_circuit_voltage =
      get_open_circuit_voltage(the_charge_supplied);
  charge_supplied = the_charge_supplied;
  open_circuit_voltage = the_open_circuit_voltage;
  return the_open_circuit_voltage;
}

FloatingPointType CellBuf::get_work() const {
  return the_cell_points_buf.lerp_work(charge_supplied);
}
