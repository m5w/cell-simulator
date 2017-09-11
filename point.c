#include "point.h"

FloatingPointType interpolate_voltage(const BatteryDischargeCurvePoint *points,
                                      const size_t number_of_points,
                                      const FloatingPointType charge) {
  const BatteryDischargeCurvePoint point = *points;
  const FloatingPointType point_charge = point.charge_discharged_from_battery;

  if (charge < point_charge) {
    error = EXTRAPOLATION_BELOW;
    const union {
      FloatingPointType interpolated_voltage;
      unsigned char s[sizeof(FloatingPointType)];
    } the_interpolated_voltage = {.s = {0}};
    return the_interpolated_voltage.interpolated_voltage;
  }

  error = NONE;
  const struct InterpolatedVoltage interpolated_voltage =
      interpolate_voltage_points_iterator(
          points, point, point_charge,
          points + number_of_points * sizeof(BatteryDischargeCurvePoint),
          charge);

  if (error == EXTRAPOLATION_ABOVE) {
    const union {
      FloatingPointType interpolated_voltage;
      unsigned char s[sizeof(FloatingPointType)];
    } the_interpolated_voltage = {.s = {0}};
    return the_interpolated_voltage.interpolated_voltage;
  }

  return interpolated_voltage.interpolated_voltage;
}

struct InterpolatedVoltage interpolate_voltage_points_iterator(
    const BatteryDischargeCurvePoint *points_iterator,
    BatteryDischargeCurvePoint point, FloatingPointType point_charge,
    const BatteryDischargeCurvePoint *const points_end,
    const FloatingPointType charge) {
  for (;;) {
    if (charge == point_charge) {
      const struct InterpolatedVoltage interpolated_voltage = {
          .interpolated_voltage = point.battery_voltage,
          .points_iterator = points_iterator,
          .point = point,
          .point_charge = point_charge};
      return interpolated_voltage;
    }

    const BatteryDischargeCurvePoint *const next_points_iterator =
        points_iterator + 1;

    if (next_points_iterator >= points_end) {
      error = EXTRAPOLATION_ABOVE;
      const union {
        FloatingPointType interpolated_voltage;
        unsigned char s[sizeof(FloatingPointType)];
      } the_interpolated_voltage = {.s = {0}};
      const union {
        BatteryDischargeCurvePoint *points_iterator;
        unsigned char s[sizeof(BatteryDischargeCurvePoint *)];
      } the_points_iterator = {.s = {0}};
      const union {
        BatteryDischargeCurvePoint point;
        unsigned char s[sizeof(BatteryDischargeCurvePoint)];
      } the_point = {.s = {0}};
      const union {
        FloatingPointType point_charge;
        unsigned char s[sizeof(FloatingPointType)];
      } the_point_charge = {.s = {0}};
      const struct InterpolatedVoltage interpolated_voltage = {
          .interpolated_voltage = the_interpolated_voltage.interpolated_voltage,
          .points_iterator = the_points_iterator.points_iterator,
          .point = the_point.point,
          .point_charge = the_point_charge.point_charge};
      return interpolated_voltage;
    }

    const BatteryDischargeCurvePoint next_point = *next_points_iterator;
    const FloatingPointType next_point_charge =
        next_point.charge_discharged_from_battery;

    if (charge < next_point_charge) {
      const FloatingPointType point_voltage = point.battery_voltage;
      const FloatingPointType next_point_voltage = next_point.battery_voltage;
      const struct InterpolatedVoltage interpolated_voltage = {
          .interpolated_voltage = point_voltage +
                                  (charge - point_charge) *
                                      (next_point_voltage - point_voltage) /
                                      (next_point_charge - point_charge),
          .points_iterator = points_iterator,
          .point = point,
          .point_charge = point_charge};
      return interpolated_voltage;
    }

    points_iterator = next_points_iterator;
    point = next_point;
    point_charge = next_point_charge;
  }
}
