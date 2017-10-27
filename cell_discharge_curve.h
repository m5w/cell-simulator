#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstddef>
#include <cstdint>

#include <istream>
#include <vector>

#include "floating_point_type.h"
#include "point.h"
#include "read.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(std::istream &is);
  inline FloatingPointType get_initial_work() const;

private:
  static inline FloatingPointType point_get_work(const Point *const point);
  Point read_point(std::istream &is);
  inline const Point *get_points() const;
  inline std::size_t get_number_of_points() const;
  std::vector<Point> the_get_point;
  std::vector<FloatingPointType> the_get_point_work;
};

FloatingPointType CellDischargeCurve::get_initial_work() const {
  return the_get_point_work.front();
}

FloatingPointType
CellDischargeCurve::point_get_work(const Point *const point) {
  return *(point->z_pointer);
}

const Point *CellDischargeCurve::get_points() const {
  return CellDischargeCurve::the_get_point.data();
}

std::size_t CellDischargeCurve::get_number_of_points() const {
  return CellDischargeCurve::the_get_point.size();
}

#endif
