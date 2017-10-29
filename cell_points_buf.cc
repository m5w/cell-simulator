#include "cell_points_buf.h"

CellPointsBuf::CellPointsBuf(const CellPoints *const the_cell_points_pointer)
    : the_cell_points_pointer(the_cell_points_pointer),
      initial_work(the_cell_points_pointer->get_initial_work()),
      buf(lerp_linear_buf(the_cell_points_pointer->get_points(),
                          the_cell_points_pointer->get_number_of_points())) {}

FloatingPointType CellPointsBuf::lerp_open_circuit_voltage(
    const FloatingPointType charge_supplied) {
  const FloatingPointType the_open_circuit_voltage =
      lerp_linear(&buf, charge_supplied);

  if (BOOST_UNLIKELY(lerp_error) != NONE)
    throw std::exception();

  return the_open_circuit_voltage;
}

FloatingPointType
CellPointsBuf::lerp_work(const FloatingPointType charge_supplied) const {
  return lerp_z_linear(&buf, charge_supplied, point_get_work);
}
