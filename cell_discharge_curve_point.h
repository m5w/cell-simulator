#ifndef CELL_DISCHARGE_CURVE_POINT_H
#define CELL_DISCHARGE_CURVE_POINT_H

#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CellDischargeCurvePoint {
  FloatingPointType charge_discharged_from_cell;
  FloatingPointType cell_voltage;
};

typedef struct CellDischargeCurvePoint CellDischargeCurvePoint;

const union {
  const CellDischargeCurvePoint cell_discharge_curve_point;
  const unsigned char s[sizeof(CellDischargeCurvePoint)];
} union_cell_discharge_curve_point = {.s = {0}};

#define ERROR_CELL_DISCHARGE_CURVE_POINT                                       \
  union_cell_discharge_curve_point.cell_discharge_curve_point;

#ifdef __cplusplus
}
#endif
#endif
