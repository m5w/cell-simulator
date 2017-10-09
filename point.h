#ifndef POINT_H
#define POINT_H

#include "floating_point_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Point {
  FloatingPointType x;
  FloatingPointType y;
} Point;

const Point default_point = {};

#ifdef __cplusplus
}

bool operator==(const Point &lhs, const Point &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
