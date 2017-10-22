#include "cell_discharge_curve.h"

CellDischargeCurve::CellDischargeCurve(std::istream &is) {
  const std::size_t number_of_points = read<std::uint64_t>(is);
  CellDischargeCurve::point_electric_potential_energy_values.reserve(
      number_of_points);
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

FloatingPointType CellDischargeCurve::lerp_electric_potential_energy(
    const FloatingPointType charge_discharged_from_cell) const {
  return lerp_z_linear(&(CellDischargeCurve::buf), charge_discharged_from_cell,
                       point_get_electric_potential_energy);
}

Point CellDischargeCurve::read_point(std::istream &is) {
  const FloatingPointType charge_discharged_from_cell = read<double>(is);
  const FloatingPointType voltage_source_voltage = read<double>(is);
  const FloatingPointType electric_potential_energy = read<double>(is);
  CellDischargeCurve::point_electric_potential_energy_values.push_back(
      electric_potential_energy);
  return {
      .x = charge_discharged_from_cell,
      .y = voltage_source_voltage,
      .z_pointer =
          &CellDischargeCurve::point_electric_potential_energy_values.back()};
}
