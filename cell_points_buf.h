#ifndef CELL_POINTS_BUF_H
#define CELL_POINTS_BUF_H

#include "floating_point_type.h"
#include "lerp.h"
#include "point.h"

#include "cell_points.h"

class CellPointsBuf {
public:
  static inline FloatingPointType
  point_get_work(const Point *const the_point_pointer);
  CellPointsBuf(const CellPoints *const the_cell_points_pointer);
  FloatingPointType
  lerp_open_circuit_voltage(const FloatingPointType charge_supplied);
  FloatingPointType lerp_work(const FloatingPointType charge_supplied) const;

private:
  const CellPoints *const the_cell_points_pointer;

public:
  const FloatingPointType initial_work;

private:
  LerpLinearBuf buf;
};

FloatingPointType
CellPointsBuf::point_get_work(const Point *const the_point_pointer) {
  return *(the_point_pointer->z_pointer);
}

#endif
