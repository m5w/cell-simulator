#include "cell_discharge_curve.h"

CellDischargeCurve::CellDischargeCurve(std::basic_istream<unsigned char> &is) {
  std::uint64_t number_of_points;
  is.read(reinterpret_cast<unsigned char *>(&number_of_points),
          sizeof(number_of_points));
  points.reserve(number_of_points);

  for (std::uint64_t points_index = 0; points_index < number_of_points;
       ++points_index)
    points.push_back(read_point(is));

  is.read(reinterpret_cast<unsigned char *>(&mean_internal_conductance),
          sizeof(mean_internal_conductance));
}

Point CellDischargeCurve::read_point(std::basic_istream<unsigned char> &is) {
  FloatingPointType x;
  is.read(reinterpret_cast<unsigned char *>(&x), sizeof(x));
  FloatingPointType y;
  is.read(reinterpret_cast<unsigned char *>(&y), sizeof(y));
  return {.x = x, .y = y};
}
