#ifndef LERP_H
#define LERP_H

#include <stddef.h>
#include <string.h>

#include "floating_point_type.h"
#include "point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum LerpLinearState {
  LINEAR,
  LINEAR_Y_CMP_FIRST_X,
  LINEAR_M_CMP_FIRST_X,
  LINEAR_Y_CMP_X,
  LINEAR_M_CMP_X
} LerpLinearState;

typedef struct LerpLinearBuf {
  LerpLinearState state;
  const Point *points_end;
  const Point *points_iterator;
  FloatingPointType point_x;
  const Point *next_points_iterator;
  FloatingPointType next_point_x;
  FloatingPointType point_y;
  FloatingPointType m;
  const Point *points_begin;
} LerpLinearBuf;

const LerpLinearBuf default_lerp_linear_buf = {};

LerpLinearBuf lerp_linear_buf(const Point *const points,
                              const size_t number_of_points);

#ifdef __cplusplus
}

bool operator==(const LerpLinearBuf &lhs, const LerpLinearBuf &rhs) {
  return lhs.state == rhs.state && lhs.points_end == rhs.points_end &&
         lhs.points_iterator == rhs.points_iterator &&
         lhs.point_x == rhs.point_x &&
         lhs.next_points_iterator == rhs.next_points_iterator &&
         lhs.next_point_x == rhs.next_point_x && lhs.point_y == rhs.point_y &&
         lhs.m == rhs.m && lhs.points_begin == rhs.points_begin;
}

extern "C" {
#endif

typedef enum LerpBinaryState {
  BINARY,
  BINARY_Y_CMP_X,
  BINARY_B_M_CMP_X,
  BINARY_A_M_CMP_X
} LerpBinaryState;

typedef struct LerpBinaryBuf {
  FloatingPointType points_front_x;
  FloatingPointType points_back_x;
  LerpBinaryState state;
  const Point *points_front_pointer;
  const Point *points_back_pointer;
  const Point *points_iterator;
  FloatingPointType point_x;
  FloatingPointType point_y;
  FloatingPointType m;
} LerpBinaryBuf;

const LerpBinaryBuf default_lerp_binary_buf = {};

LerpBinaryBuf lerp_binary_buf(const Point *const points,
                              const size_t number_of_points);

#ifdef __cplusplus
}

bool operator==(const LerpBinaryBuf &lhs, const LerpBinaryBuf &rhs) {
  return lhs.points_front_x == rhs.points_front_x &&
         lhs.points_back_x == rhs.points_back_x && lhs.state == rhs.state &&
         lhs.points_front_pointer == rhs.points_front_pointer &&
         lhs.points_back_pointer == rhs.points_back_pointer &&
         lhs.points_iterator == rhs.points_iterator &&
         lhs.point_x == rhs.point_x && lhs.point_y == rhs.point_y &&
         lhs.m == rhs.m;
}

extern "C" {
#endif

typedef enum LerpError {
  NONE,
  EXTRAPOLATION,
  EXTRAPOLATION_BELOW,
  EXTRAPOLATION_ABOVE
} LerpError;

extern LerpError lerp_error;

FloatingPointType lerp_linear(LerpLinearBuf *const buf_pointer,
                              const FloatingPointType x);

FloatingPointType lerp_binary(LerpBinaryBuf *const buf_pointer,
                              const FloatingPointType x);

static inline FloatingPointType get_m(const FloatingPointType point_x,
                                      const Point *const next_points_iterator,
                                      const FloatingPointType next_point_x,
                                      const FloatingPointType point_y);

static inline FloatingPointType lerp(const FloatingPointType x_1,
                                     const FloatingPointType y_1,
                                     const FloatingPointType m,
                                     const FloatingPointType x);

FloatingPointType get_m(const FloatingPointType point_x,
                        const Point *const next_points_iterator,
                        const FloatingPointType next_point_x,
                        const FloatingPointType point_y) {
  return (next_points_iterator->y - point_y) / (next_point_x - point_x);
}

FloatingPointType lerp(const FloatingPointType x_1,
                       const FloatingPointType y_1, const FloatingPointType m,
                       const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}

#ifdef __cplusplus
}
#endif
#endif
