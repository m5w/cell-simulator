#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ErrorType {
  NONE,
  EXTRAPOLATION,
  EXTRAPOLATION_BELOW,
  EXTRAPOLATION_ABOVE
} ErrorType;

extern ErrorType error;

#ifdef __cplusplus
}
#endif
#endif
