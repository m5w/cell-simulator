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

class CellDischargeCurve {
public:
  CellDischargeCurve(std::istream &is);
  FloatingPointType lerp_voltage_source_voltage(
      const FloatingPointType charge_discharged_from_cell) const;
  FloatingPointType lerp_electric_potential_energy(
      const FloatingPointType charge_discharged_from_cell) const;

private:
  template <class T> static inline T read(std::istream &is);
  Point read_point(std::istream &is);
  static inline FloatingPointType
  point_get_electric_potential_energy(const Point *const point);
  inline const Point *get_points() const;
  inline std::size_t get_number_of_points() const;
  std::vector<FloatingPointType> point_electric_potential_energy_values;
  std::vector<Point> points;
  mutable LerpLinearBuf buf;
};

template <class T> T CellDischargeCurve::read(std::istream &is) {
  T is_x;
  is.read(reinterpret_cast<char *>(&is_x), sizeof(is_x));
  return is_x;
}

FloatingPointType CellDischargeCurve::point_get_electric_potential_energy(
    const Point *const point) {
  return *(point->z_pointer);
}

const Point *CellDischargeCurve::get_points() const {
  return CellDischargeCurve::points.data();
}

std::size_t CellDischargeCurve::get_number_of_points() const {
  return CellDischargeCurve::points.size();
}

#endif
