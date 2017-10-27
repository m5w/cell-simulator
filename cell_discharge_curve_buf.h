#ifndef CELL_DISCHARGE_CURVE_BUF_H
#define CELL_DISCHARGE_CURVE_BUF_H

#include <exception>

#include <boost/config.hpp>

#include "cell_discharge_curve.h"
#include "lerp.h"

class CellDischargeCurveBuf {
public:
  CellDischargeCurveBuf(const CellDischargeCurve &the_cell_discharge_curve);
  inline FloatingPointType get_initial_work() const;
  FloatingPointType
  lerp_open_circuit_voltage(const FloatingPointType charge_supplied);
  FloatingPointType lerp_work(const FloatingPointType charge_supplied) const;

private:
  static inline FloatingPointType point_get_work(const Point *const the_point);
  const CellDischargeCurve &the_cell_discharge_curve;
  LerpLinearBuf buf;
};

FloatingPointType CellDischargeCurveBuf::get_initial_work() const {
  return the_cell_discharge_curve.the_get_point_work.front();
}

FloatingPointType
CellDischargeCurveBuf::point_get_work(const Point *const the_point) {
  return *(the_point->z_pointer);
}

#endif
