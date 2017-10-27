#ifndef CELL_H
#define CELL_H

#include <istream>

#include "cell_discharge_curve.h"
#include "floating_point_type.h"
#include "read.h"

class Cell {
public:
  Cell(const CellDischargeCurve &discharge_curve,
       const FloatingPointType mean_internal_conductance);
  Cell(const Cell &rhs, FloatingPointType &voltage_source_voltage);
  inline FloatingPointType get_initial_work() const;
  FloatingPointType
  get_next_voltage_source_voltage(const FloatingPointType change_in_time,
                                  const FloatingPointType module_voltage);
  FloatingPointType get_work() const;

private:
  inline FloatingPointType
  get_voltage_source_voltage(const FloatingPointType charge_supplied);
  inline FloatingPointType get_mean_internal_conductance() const;
  CellDischargeCurve discharge_curve;
  const FloatingPointType mean_internal_conductance;
  FloatingPointType charge_supplied;
  FloatingPointType voltage_source_voltage;
};

Cell read_cell(std::istream &is, const CellDischargeCurve &discharge_curve,
               FloatingPointType &mean_internal_conductance);

FloatingPointType Cell::get_initial_work() const {
  return Cell::discharge_curve.get_initial_work();
}

FloatingPointType
Cell::get_voltage_source_voltage(const FloatingPointType charge_supplied) {
  return Cell::discharge_curve.lerp_voltage_source_voltage(charge_supplied);
}

FloatingPointType Cell::get_mean_internal_conductance() const {
  return Cell::mean_internal_conductance;
}

#endif
