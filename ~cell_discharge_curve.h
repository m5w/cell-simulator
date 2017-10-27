#ifndef CELL_DISCHARGE_CURVE_H
#define CELL_DISCHARGE_CURVE_H

#include <cstddef>
#include <cstdint>

#include <exception>
#include <istream>
#include <vector>

#include <boost/config.hpp>

#include "floating_point_type.h"
#include "lerp.h"
#include "point.h"
#include "read.h"

class CellDischargeCurve {
public:
  CellDischargeCurve(const std::vector<FloatingPointType> &point_work_values,
                     const std::vector<Point> &points);
  inline FloatingPointType get_initial_work() const;
  FloatingPointType lerp_voltage_source_voltage(
      const FloatingPointType charge_discharged_from_cell) const;
  FloatingPointType
  lerp_work(const FloatingPointType charge_discharged_from_cell) const;

private:
  static inline FloatingPointType point_get_work(const Point *const point);
  inline const Point *get_points() const;
  inline std::size_t get_number_of_points() const;
  const std::vector<FloatingPointType> &point_work_values;
  const std::vector<Point> &points;
  mutable LerpLinearBuf buf;
};

CellDischargeCurve
read_cell_discharge_curve(std::istream &is,
                          std::vector<FloatingPointType> &point_work_values,
                          std::vector<Point> &points);
static Point read_point(std::istream &is,
                        std::vector<FloatingPointType> &point_work_values,
                        std::vector<Point> &points);

FloatingPointType CellDischargeCurve::get_initial_work() const {
  return point_work_values.front();
}

FloatingPointType
CellDischargeCurve::point_get_work(const Point *const point) {
  return *(point->z_pointer);
}

const Point *CellDischargeCurve::get_points() const {
  return CellDischargeCurve::points.data();
}

std::size_t CellDischargeCurve::get_number_of_points() const {
  return CellDischargeCurve::points.size();
}

#endif
