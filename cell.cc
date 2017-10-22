#include "cell.h"

Cell::Cell(std::istream &is, FloatingPointType &mean_internal_conductance,
           FloatingPointType &voltage_source_voltage)
    : discharge_curve(is) {
  double is_mean_internal_conductance;
  is.read(reinterpret_cast<char *>(&is_mean_internal_conductance),
          sizeof(is_mean_internal_conductance));
  const FloatingPointType the_mean_internal_conductance =
      is_mean_internal_conductance;
  Cell::mean_internal_conductance = is_mean_internal_conductance;
  mean_internal_conductance = the_mean_internal_conductance;

  const FloatingPointType the_charge_discharged_from_cell = 0.0;
  const FloatingPointType the_voltage_source_voltage =
      get_voltage_source_voltage(the_charge_discharged_from_cell);
  Cell::charge_discharged_from_cell = the_charge_discharged_from_cell;
  Cell::voltage_source_voltage = the_voltage_source_voltage;
  voltage_source_voltage = the_voltage_source_voltage;
}

FloatingPointType
Cell::get_next_voltage_source_voltage(const FloatingPointType change_in_time,
                                      const FloatingPointType module_voltage) {
  const FloatingPointType the_charge_discharged_from_cell =
      Cell::charge_discharged_from_cell +
      (Cell::voltage_source_voltage - module_voltage) *
          Cell::mean_internal_conductance * change_in_time;
  const FloatingPointType the_voltage_source_voltage =
      get_voltage_source_voltage(the_charge_discharged_from_cell);
  Cell::charge_discharged_from_cell = the_charge_discharged_from_cell;
  Cell::voltage_source_voltage = the_voltage_source_voltage;
  return the_voltage_source_voltage;
}

FloatingPointType Cell::get_electric_potential_energy() const {
  return Cell::discharge_curve.lerp_electric_potential_energy(
      Cell::charge_discharged_from_cell);
}
