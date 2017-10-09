#ifndef FLOATING_POINT_TYPE_H
#define FLOATING_POINT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef double FloatingPointType;

const struct {
  FloatingPointType default_floating_point_type;
} default_floating_point_type = {};

#define DEFAULT_FLOATING_POINT_TYPE                                           \
  default_floating_point_type.default_floating_point_type

#ifdef __cplusplus
}
#endif
#endif
