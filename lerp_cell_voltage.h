#ifndef LERP_CELL_VOLTAGE_H
#define LERP_CELL_VOLTAGE_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

enum LerpCellVoltageLinearForwardStateType {
  LINEAR_FORWARD,
  LINEAR_FORWARD_VOLTAGE_CMP_FIRST_CHARGE,
  LINEAR_FORWARD_M_CMP_FIRST_CHARGE,
  LINEAR_FORWARD_VOLTAGE_CMP_CHARGE,
  LINEAR_FORWARD_M_CMP_CHARGE
};

enum LerpCellVoltageLinearStateType {
  LINEAR,
  LINEAR_VOLTAGE_CMP_FIRST_CHARGE,
  LINEAR_M_CMP_FIRST_CHARGE,
  LINEAR_VOLTAGE_CMP_CHARGE,
  LINEAR_M_CMP_CHARGE
};

typedef struct LerpCellVoltageLinearForwardBufType {
  enum LerpCellVoltageLinearForwardStateType state;
  const CellDischargeCurvePoint *const points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
} LerpCellVoltageLinearForwardBufType;

typedef struct LerpCellVoltageLinearBufType {
  enum LerpCellVoltageLinearStateType state;
  const CellDischargeCurvePoint *const points_begin;
  const CellDischargeCurvePoint *const points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
} LerpCellVoltageLinearBufType;

LerpCellVoltageLinearForwardBufType lerp_cell_voltage_linear_forward_buf_type(
    const CellDischargeCurvePoint *const points,
    const size_t number_of_points);

LerpCellVoltageLinearBufType
lerp_cell_voltage_linear_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points);

inline FloatingPointType lerp_cell_voltage(void *const buf_pointer,
                                           const FloatingPointType charge);

FloatingPointType lerp_cell_voltage_linear_forward(
    LerpCellVoltageLinearForwardBufType *const buf_pointer,
    const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_linear(LerpCellVoltageLinearBufType *const buf_pointer,
                         const FloatingPointType charge);

static inline FloatingPointType lerp_cell_voltage_get_m(
    const FloatingPointType point_charge,
    const CellDischargeCurvePoint *const next_points_iterator,
    const FloatingPointType next_point_charge,
    const FloatingPointType point_voltage);

static FloatingPointType lerp(const FloatingPointType x_1,
                              const FloatingPointType y_1,
                              const FloatingPointType m,
                              const FloatingPointType x);

FloatingPointType lerp_cell_voltage(void *const buf_pointer,
                                    const FloatingPointType charge) {
  return lerp_cell_voltage_linear_forward(
      (LerpCellVoltageLinearForwardBufType *)buf_pointer, charge);
}

#ifdef __cplusplus
}
#endif
#endif
