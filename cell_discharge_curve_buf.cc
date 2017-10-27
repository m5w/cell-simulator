#include "cell_discharge_curve_buf.h"

CellDischargeCurveBuf::CellDischargeCurveBuf(
    const CellDischargeCurve &the_cell_discharge_curve)
    : the_cell_discharge_curve(the_cell_discharge_curve),
      buf(lerp_linear_buf(the_cell_discharge_curve.get_points(),
                          the_cell_discharge_curve.get_number_of_points())) {}

FloatingPointType CellDischargeCurveBuf::lerp_open_circuit_voltage(
    const FloatingPointType charge_supplied) {
  const FloatingPointType open_circuit_voltage =
      lerp_linear(&(CellDischargeCurveBuf::buf), charge_supplied);

  if (BOOST_UNLIKELY(lerp_error != NONE))
    throw std::exception();

  return open_circuit_voltage;
}

FloatingPointType CellDischargeCurveBuf::lerp_work(
    const FloatingPointType charge_supplied) const {
  const FloatingPointType work = lerp_z_linear(
      &(CellDischargeCurveBuf::buf), charge_supplied, point_get_work);

  if (BOOST_UNLIKELY(lerp_error != NONE))
    throw std::exception();

  return work;
}
