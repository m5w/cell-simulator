#ifndef INTERPOLATE_CELL_VOLTAGE_H
#define INTERPOLATE_CELL_VOLTAGE_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

enum LerpCellVoltageStateType {
  LERP_CELL_VOLTAGE,
  CMP_CHARGE,
  CMP_NEXT_CHARGE
};

typedef struct LerpCellVoltageType {
  enum LerpCellVoltageStateType state;
  const CellDischargeCurvePoint *const points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
} LerpCellVoltageType;

LerpCellVoltageType
lerp_cell_voltage_state_type(const CellDischargeCurvePoint *const points,
                             const size_t number_of_points);

inline FloatingPointType
lerp_cell_voltage(LerpCellVoltageType *const state_pointer,
                  const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_linear_forward(LerpCellVoltageType *const state_pointer,
                                 const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_linear(LerpCellVoltageType *const state_pointer,
                         const FloatingPointType charge);

static FloatingPointType lerp(const FloatingPointType x_1,
                              const FloatingPointType y_1,
                              const FloatingPointType m,
                              const FloatingPointType x);

static inline FloatingPointType
lerp_cell_voltage_lerp(LerpCellVoltageType *const state_pointer,
                       const FloatingPointType charge);

FloatingPointType lerp_cell_voltage(LerpCellVoltageType *const state_pointer,
                                    const FloatingPointType charge) {
  return lerp_cell_voltage_linear_forward(state_pointer, charge);
}

/* Linear intERPolation
 *
 * Assuming that the voltage is set, set the slope and linearly interpolate the
 * voltage between the point and the next point at the given charge.  Set the
 * coroutine to re-enter at CMP_NEXT_CHARGE.
 */
FloatingPointType
lerp_cell_voltage_lerp(LerpCellVoltageType *const state_pointer,
                       const FloatingPointType charge) {
  state_pointer->m =
      (state_pointer->next_points_iterator->cell_voltage -
       state_pointer->point_voltage) /
      (state_pointer->next_point_charge - state_pointer->point_charge);
  state_pointer->state = CMP_NEXT_CHARGE;
  return lerp(state_pointer->point_charge, state_pointer->point_voltage,
              state_pointer->m, charge);
}

#ifdef __cplusplus
}
#endif
#endif
