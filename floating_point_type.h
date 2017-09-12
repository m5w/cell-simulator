#ifndef FLOATING_POINT_TYPE_H
#define FLOATING_POINT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef float FloatingPointType;

FloatingPointType error_floating_point_type;

const union {
  const FloatingPointType floating_point_type;
  const unsigned char s[sizeof(FloatingPointType)];
} union_floating_point_type = {.s = {0}};

#define ERROR_FLOATING_POINT_TYPE union_floating_point_type.floating_point_type;

#ifdef __cplusplus
}
#endif
#endif
