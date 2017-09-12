#ifndef CELL_VOLTAGE_INTERPOLATOR_H
#define CELL_VOLTAGE_INTERPOLATOR_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CellVoltageInterpolator {
  const CellDischargeCurvePoint *points_iterator;
  const CellDischargeCurvePoint *const points_end;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
};

typedef struct CellVoltageInterpolator CellVoltageInterpolator;

static const CellVoltageInterpolator error_cell_voltage_interpolator;

struct InterpolatedVoltage {
  FloatingPointType interpolated_voltage;
  CellVoltageInterpolator interpolator;
};

CellVoltageInterpolator
cell_voltage_interpolator_iter(const CellDischargeCurvePoint *const points,
                               const size_t number_of_points,
                               const FloatingPointType charge);

FloatingPointType cell_voltage_interpolator_next(
    CellVoltageInterpolator *const interpolator_pointer,
    const FloatingPointType charge);

#ifdef __cplusplus
}
#endif
#endif
