#ifndef INTERPOLATE_CELL_VOLTAGE_H
#define INTERPOLATE_CELL_VOLTAGE_H

#include <stddef.h>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

FloatingPointType
interpolate_cell_voltage(const CellDischargeCurvePoint *const points,
                         const size_t number_of_points,
                         const FloatingPointType charge);

static inline FloatingPointType interpolate(const FloatingPointType x_0,
                                            const FloatingPointType v_0,
                                            const FloatingPointType slope,
                                            const FloatingPointType x_q);

#ifdef __cplusplus
}
#endif
#endif
