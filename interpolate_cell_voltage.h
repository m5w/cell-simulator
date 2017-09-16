#ifndef INTERPOLATE_CELL_VOLTAGE_H
#define INTERPOLATE_CELL_VOLTAGE_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LerpCellVoltageStateType {
  enum { LERP_CELL_VOLTAGE, CMP_CHARGE, CMP_NEXT_CHARGE } state;
  const CellDischargeCurvePoint *const points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
} LerpCellVoltageStateType;

LerpCellVoltageStateType
lerp_cell_voltage_state_type(const CellDischargeCurvePoint *const points,
                             const size_t number_of_points);

inline FloatingPointType
lerp_cell_voltage(LerpCellVoltageStateType *const state_pointer,
                  const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_linear_forward(LerpCellVoltageStateType *const state_pointer,
                                 const FloatingPointType charge);

static FloatingPointType lerp(const FloatingPointType x_1,
                              const FloatingPointType y_1,
                              const FloatingPointType m,
                              const FloatingPointType x);

inline FloatingPointType
lerp_cell_voltage(LerpCellVoltageStateType *const state_pointer,
                  const FloatingPointType charge) {
  return lerp_cell_voltage_linear_forward(state_pointer, charge);
}

#ifdef __cplusplus
}
#endif
#endif
