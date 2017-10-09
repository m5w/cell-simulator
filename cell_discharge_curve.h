#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstdint>
#include <istream>
#include <vector>

#include "floating_point_type.h"
#include "point.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(std::basic_istream<unsigned char> &is);

private:
  Point read_point(std::basic_istream<unsigned char> &is);
  std::vector<Point> points;
  FloatingPointType mean_internal_conductance;
};

#endif
