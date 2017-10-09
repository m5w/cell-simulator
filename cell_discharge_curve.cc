#include "cell_discharge_curve.h"

CellDischargeCurve::CellDischargeCurve(std::istream &is) {
  std::uint64_t number_of_points;
  is.read(reinterpret_cast<char *>(&number_of_points),
          sizeof(number_of_points));
  points.reserve(number_of_points);

  for (std::uint64_t points_index = 0; points_index < number_of_points;
       ++points_index)
    points.push_back(read_point(is));

  is.read(reinterpret_cast<char *>(&mean_internal_conductance),
          sizeof(mean_internal_conductance));
}

const Point *CellDischargeCurve::get_points() const { return points.data(); }

const std::size_t CellDischargeCurve::get_number_of_points() const {
  return points.size();
}

const FloatingPointType
CellDischargeCurve::get_mean_internal_conductance() const {
  return mean_internal_conductance;
}

Point CellDischargeCurve::read_point(std::istream &is) {
  FloatingPointType x;
  is.read(reinterpret_cast<char *>(&x), sizeof(x));
  FloatingPointType y;
  is.read(reinterpret_cast<char *>(&y), sizeof(y));
  return {.x = x, .y = y};
}
