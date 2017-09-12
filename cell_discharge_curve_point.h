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

#ifdef __cplusplus
}
#endif
#endif
