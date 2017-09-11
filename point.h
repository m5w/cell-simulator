#ifndef GTSR_BATTERY_SIMULATOR_POINT_H
#define GTSR_BATTERY_SIMULATOR_POINT_H

#include <stddef.h>
#include <stdlib.h>

#include "error.h"

typedef long double FloatingPointType;

struct BatteryDischargeCurvePoint {
  FloatingPointType charge_discharged_from_battery;
  FloatingPointType battery_voltage;
};

typedef struct BatteryDischargeCurvePoint BatteryDischargeCurvePoint;

FloatingPointType interpolate_voltage(const BatteryDischargeCurvePoint *points,
                                      const size_t number_of_points,
                                      const FloatingPointType charge);

struct InterpolatedVoltage {
  FloatingPointType interpolated_voltage;
  const BatteryDischargeCurvePoint *points_iterator;
  BatteryDischargeCurvePoint point;
  FloatingPointType point_charge;
} interpolate_voltage_points_iterator(
    const BatteryDischargeCurvePoint *points_iterator,
    BatteryDischargeCurvePoint point, FloatingPointType point_charge,
    const BatteryDischargeCurvePoint *const points_end,
    const FloatingPointType charge);

#endif
