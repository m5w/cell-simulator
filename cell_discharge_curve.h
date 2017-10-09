#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

#include "floating_point_type.h"
#include "point.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(std::istream &is);
  const Point *get_points() const;
  const std::size_t get_number_of_points() const;
  const FloatingPointType get_mean_internal_conductance() const;

private:
  Point read_point(std::istream &is);
  std::vector<Point> points;
  FloatingPointType mean_internal_conductance;
};

#endif
