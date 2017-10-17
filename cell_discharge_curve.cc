#include "cell_discharge_curve.h"

Point CellDischargeCurve::read_point(std::istream &is) {
  double is_x;
  is.read(reinterpret_cast<char *>(&is_x), sizeof(is_x));
  const FloatingPointType x = is_x;
  double is_y;
  is.read(reinterpret_cast<char *>(&is_y), sizeof(is_y));
  const FloatingPointType y = is_y;
  return {.x = x, .y = y};
}

CellDischargeCurve::CellDischargeCurve(std::istream &is) {
  std::uint64_t is_number_of_points;
  is.read(reinterpret_cast<char *>(&is_number_of_points),
          sizeof(is_number_of_points));
  const std::size_t number_of_points = is_number_of_points;
  CellDischargeCurve::points.reserve(number_of_points);

  for (std::size_t points_index = 0; points_index < number_of_points;
       ++points_index)
    CellDischargeCurve::points.push_back(read_point(is));

  CellDischargeCurve::buf =
      lerp_linear_buf(get_points(), get_number_of_points());
}

FloatingPointType CellDischargeCurve::lerp_voltage_source_voltage(
    const FloatingPointType charge_discharged_from_cell) const {
  const FloatingPointType voltage_source_voltage =
      lerp_linear(&(CellDischargeCurve::buf), charge_discharged_from_cell);

  if (BOOST_UNLIKELY(lerp_error != NONE))
    throw;

  return voltage_source_voltage;
}
