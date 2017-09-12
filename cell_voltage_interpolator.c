#include "cell_voltage_interpolator.h"

static const union {
  const CellDischargeCurvePoint *const points_iterator;
  const unsigned char s[sizeof(CellDischargeCurvePoint *)];
} error_points_iterator = {.s = {0}};
static const union {
  const CellDischargeCurvePoint *const points_end;
  const unsigned char s[sizeof(CellDischargeCurvePoint *)];
} error_points_end = {.s = {0}};
static const union {
  const FloatingPointType point_charge;
  const unsigned char s[sizeof(FloatingPointType)];
} error_point_charge = {.s = {0}};
static const union {
  const CellDischargeCurvePoint *const next_points_iterator;
  const unsigned char s[sizeof(CellDischargeCurvePoint *)];
} error_next_points_iterator = {.s = {0}};
static const union {
  const FloatingPointType next_point_charge;
  const unsigned char s[sizeof(FloatingPointType)];
} error_next_point_charge = {.s = {0}};
const CellVoltageInterpolator error_cell_voltage_interpolator = {
    .points_iterator = error_points_iterator.points_iterator,
    .points_end = error_points_end.points_end,
    .point_charge = error_point_charge.point_charge,
    .next_points_iterator = error_next_points_iterator.next_points_iterator,
    .next_point_charge = error_next_point_charge.next_point_charge};

CellVoltageInterpolator
cell_voltage_interpolator_iter(const CellDischargeCurvePoint *const points,
                               const size_t number_of_points,
                               const FloatingPointType charge) {
  const CellDischargeCurvePoint *const points_end = points + number_of_points;

  // This would occur if there were not any points.
  if (points >= points_end) {
    error = EXTRAPOLATION;
    return error_cell_voltage_interpolator;
  }

  const FloatingPointType point_charge = points->charge_discharged_from_cell;

  if (charge < point_charge) {
    error = EXTRAPOLATION_BELOW;
    return error_cell_voltage_interpolator;
  }

  if (charge == point_charge) {
    const CellVoltageInterpolator interpolator;

  const CellDischargeCurvePoint *const next_points_iterator = points + 1;

  if (next_points_iterator >= points_end) {
    error = EXTRAPOLATION_ABOVE;
    return error_cell_voltage_interpolator;
  }

  const CellVoltageInterpolator cell_voltage_interpolator = {
      .points_iterator = points,
      .point_charge = point_charge,
      .points_end = points + number_of_points,
      .next_points_iterator = next_points_iterator,
      .next_point_charge = next_points_iterator->charge_discharged_from_cell};
  return cell_voltage_interpolator;
}

FloatingPointType cell_voltage_interpolator_next(
    CellVoltageInterpolator *const interpolator_pointer,
    const FloatingPointType charge) {
  const CellDischargeCurvePoint *points_iterator =
      interpolator_pointer->points_iterator;
  FloatingPointType point_charge = interpolator_pointer->point_charge;
  const CellDischargeCurvePoint *const points_end =
      interpolator_pointer->points_end;
  const CellDischargeCurvePoint *next_points_iterator =
      interpolator_pointer->next_points_iterator;
  FloatingPointType next_point_charge = interpolator_pointer->next_point_charge;
}
