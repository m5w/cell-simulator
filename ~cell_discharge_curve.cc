#include "cell_discharge_curve.h"

CellDischargeCurve::CellDischargeCurve(
    const std::vector<FloatingPointType> &point_work_values,
    const std::vector<Point> &points)
    : point_work_values(point_work_values), points(points),
      buf(lerp_linear_buf(get_points(), get_number_of_points())) {}

FloatingPointType CellDischargeCurve::lerp_voltage_source_voltage(
    const FloatingPointType charge_discharged_from_cell) const {
  const FloatingPointType voltage_source_voltage =
      lerp_linear(&(CellDischargeCurve::buf), charge_discharged_from_cell);

  if (BOOST_UNLIKELY(lerp_error != NONE))
    throw;

  return voltage_source_voltage;
}

FloatingPointType CellDischargeCurve::lerp_work(
    const FloatingPointType charge_discharged_from_cell) const {
  return lerp_z_linear(&(CellDischargeCurve::buf), charge_discharged_from_cell,
                       point_get_work);
}

CellDischargeCurve
read_cell_discharge_curve(std::istream &is,
                          std::vector<FloatingPointType> &point_work_values,
                          std::vector<Point> &points) {
  const std::size_t number_of_points = read<std::uint64_t>(is);
  point_work_values.reserve(number_of_points);
  points.reserve(number_of_points);

  for (std::size_t points_index = 0; points_index < number_of_points;
       ++points_index)
    points.push_back(read_point(is, point_work_values, points));

  return CellDischargeCurve(point_work_values, points);
}

Point read_point(std::istream &is,
                 std::vector<FloatingPointType> &point_work_values,
                 std::vector<Point> &points) {
  const FloatingPointType charge_supplied = read<double>(is);
  const FloatingPointType voltage_source_voltage = read<double>(is);
  const FloatingPointType work = read<double>(is);
  point_work_values.push_back(work);
  return {.x = charge_supplied,
          .y = voltage_source_voltage,
          .z_pointer = &point_work_values.back()};
}
