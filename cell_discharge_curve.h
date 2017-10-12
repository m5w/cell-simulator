#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

#include <boost/config.hpp>

#include "floating_point_type.h"
#include "lerp.h"
#include "point.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(std::istream &is);
  FloatingPointType lerp_voltage_source_voltage(
      const FloatingPointType charge_discharged_from_cell) const;

private:
  static Point read_point(std::istream &is);
  std::vector<Point> points;
  mutable LerpLinearBuf buf;
  inline const Point *get_points() const;
  inline std::size_t get_number_of_points() const;
};

const Point *CellDischargeCurve::get_points() const { return points.data(); }

std::size_t CellDischargeCurve::get_number_of_points() const {
  return points.size();
}

#endif
