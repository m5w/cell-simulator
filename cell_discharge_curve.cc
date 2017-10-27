#include "cell_discharge_curve.h"

CellDischargeCurve::CellDischargeCurve(std::istream &is) {
  const std::size_t number_of_points = read<std::uint64_t>(is);
  CellDischargeCurve::the_get_point.reserve(number_of_points);
  CellDischargeCurve::the_get_point_work.reserve(number_of_points);

  for (std::size_t point_index = 0; point_index < number_of_points;
       ++point_index)
    CellDischargeCurve::the_get_point.push_back(read_point(is));
}

Point CellDischargeCurve::read_point(std::istream &is) {
  const FloatingPointType charge_supplied = read<double>(is);
  const FloatingPointType open_circuit_voltage = read<double>(is);
  const FloatingPointType work = read<double>(is);
  CellDischargeCurve::the_get_point_work.push_back(work);
  return {.x = charge_supplied,
          .y = open_circuit_voltage,
          .z_pointer = &the_get_point_work.back()};
}
