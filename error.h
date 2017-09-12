#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

enum ErrorType {
  NONE,
  EXTRAPOLATION,
  EXTRAPOLATION_BELOW,
  EXTRAPOLATION_ABOVE
};

extern enum ErrorType error;

#ifdef __cplusplus
}
#endif
#endif
