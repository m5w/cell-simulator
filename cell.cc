#include "cell.h"

Cell::Cell(const CellDischargeCurve &discharge_curve,
           const FloatingPointType mean_internal_conductance)
    : discharge_curve(discharge_curve),
      mean_internal_conductance(mean_internal_conductance) {
  const FloatingPointType the_charge_supplied = 0.0;
  const FloatingPointType the_voltage_source_voltage =
      get_voltage_source_voltage(the_charge_supplied);
  Cell::charge_supplied = the_charge_supplied;
  Cell::voltage_source_voltage = the_voltage_source_voltage;
}

Cell::Cell(const Cell &rhs, FloatingPointType &voltage_source_voltage)
    : discharge_curve(rhs.discharge_curve),
      mean_internal_conductance(rhs.mean_internal_conductance),
      charge_supplied(rhs.charge_supplied) {
  const FloatingPointType the_voltage_source_voltage =
      rhs.voltage_source_voltage;
  Cell::voltage_source_voltage = the_voltage_source_voltage;
  voltage_source_voltage = the_voltage_source_voltage;
}

FloatingPointType
Cell::get_next_voltage_source_voltage(const FloatingPointType change_in_time,
                                      const FloatingPointType module_voltage) {
  const FloatingPointType the_charge_supplied =
      Cell::charge_supplied +
      (Cell::voltage_source_voltage - module_voltage) *
          Cell::mean_internal_conductance * change_in_time;
  const FloatingPointType the_voltage_source_voltage =
      get_voltage_source_voltage(the_charge_supplied);
  Cell::charge_supplied = the_charge_supplied;
  Cell::voltage_source_voltage = the_voltage_source_voltage;
  return the_voltage_source_voltage;
}

FloatingPointType Cell::get_work() const {
  return Cell::discharge_curve.lerp_work(Cell::charge_supplied);
}

Cell read_cell(std::istream &is, const CellDischargeCurve &discharge_curve,
               FloatingPointType &mean_internal_conductance) {
  const FloatingPointType the_mean_internal_conductance = read<double>(is);
  mean_internal_conductance = the_mean_internal_conductance;
  return Cell(discharge_curve, the_mean_internal_conductance);
}
