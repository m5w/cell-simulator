#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstdint>
#include <istream>
#include <vector>

#include "floating_point_type.h"
#include "point.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(std::istream &is);

private:
  Point read_point(std::istream &is);
  std::vector<Point> points;
  FloatingPointType mean_internal_conductance;
};

#endif
