#ifndef CELL_H
#define CELL_H

#include <istream>

#include "cell_discharge_curve.h"
#include "floating_point_type.h"

class Cell {
public:
  Cell(std::istream &is, FloatingPointType &mean_internal_conductance,
       FloatingPointType &voltage_source_voltage);
  FloatingPointType
  get_next_voltage_source_voltage(const FloatingPointType change_in_time,
                                  const FloatingPointType module_voltage);

private:
  inline FloatingPointType get_voltage_source_voltage(
      const FloatingPointType charge_discharged_from_cell);
  CellDischargeCurve discharge_curve;
  FloatingPointType mean_internal_conductance;
  FloatingPointType charge_discharged_from_cell;
  FloatingPointType voltage_source_voltage;
};

FloatingPointType Cell::get_voltage_source_voltage(
    const FloatingPointType charge_discharged_from_cell) {
  return Cell::discharge_curve.lerp_voltage_source_voltage(
      charge_discharged_from_cell);
}

#endif