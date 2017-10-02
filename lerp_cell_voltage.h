#ifndef LERP_CELL_VOLTAGE_H
#define LERP_CELL_VOLTAGE_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

enum LerpCellVoltageLinearStateType {
  LINEAR,
  LINEAR_VOLTAGE_CMP_FIRST_CHARGE,
  LINEAR_M_CMP_FIRST_CHARGE,
  LINEAR_VOLTAGE_CMP_CHARGE,
  LINEAR_M_CMP_CHARGE
};

enum LerpCellVoltageBinaryStateType {
  BINARY,
  BINARY_VOLTAGE_CMP_CHARGE,
  BINARY_B_M_CMP_CHARGE,
  BINARY_A_M_CMP_CHARGE
};

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

const union {
  const LerpCellVoltageLinearBufType lerp_cell_voltage_linear_buf_type;
  const unsigned char s[sizeof(LerpCellVoltageLinearBufType)];
} union_lerp_cell_voltage_linear_buf_type = {.s = {0}};

#define ERROR_LERP_CELL_VOLTAGE_LINEAR_BUF_TYPE                               \
  union_lerp_cell_voltage_linear_buf_type.lerp_cell_voltage_linear_buf_type

typedef struct LerpCellVoltageBinaryBufType {
  const FloatingPointType points_front_charge;
  const FloatingPointType points_back_charge;
  enum LerpCellVoltageBinaryStateType state;
  const CellDischargeCurvePoint *const points_front_pointer;
  const CellDischargeCurvePoint *const points_back_pointer;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
} LerpCellVoltageBinaryBufType;

const union {
  const LerpCellVoltageBinaryBufType lerp_cell_voltage_binary_buf_type;
  const unsigned char s[sizeof(LerpCellVoltageBinaryBufType)];
} union_lerp_cell_voltage_binary_buf_type = {.s = {0}};

#define ERROR_LERP_CELL_VOLTAGE_BINARY_BUF_TYPE                               \
  union_lerp_cell_voltage_binary_buf_type.lerp_cell_voltage_binary_buf_type

LerpCellVoltageLinearBufType
lerp_cell_voltage_linear_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points);

LerpCellVoltageBinaryBufType
lerp_cell_voltage_binary_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points);

inline FloatingPointType lerp_cell_voltage(void *const buf_pointer,
                                           const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_linear(LerpCellVoltageLinearBufType *const buf_pointer,
                         const FloatingPointType charge);

FloatingPointType
lerp_cell_voltage_binary(LerpCellVoltageBinaryBufType *const buf_pointer,
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
  return lerp_cell_voltage_linear((LerpCellVoltageLinearBufType *)buf_pointer,
                                  charge);
}

FloatingPointType lerp_cell_voltage_get_m(
    const FloatingPointType point_charge,
    const CellDischargeCurvePoint *const next_points_iterator,
    const FloatingPointType next_point_charge,
    const FloatingPointType point_voltage) {
  return (next_points_iterator->cell_voltage - point_voltage) /
         (next_point_charge - point_charge);
}

#ifdef __cplusplus
}
#endif
#endif
