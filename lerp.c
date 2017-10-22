#include "lerp.h"

LerpLinearBuf lerp_linear_buf(const Point *const points,
                              const size_t number_of_points) {
  if (number_of_points < 2) {
    lerp_error = NUMBER_OF_POINTS_BELOW_TWO;
    return default_lerp_linear_buf;
  }

  LerpLinearBuf buf = default_lerp_linear_buf;
  buf.state = LINEAR;
  buf.points_end = points + number_of_points;
  buf.points_iterator = points;
  buf.points_begin = points;
  return buf;
}

LerpBinaryBuf lerp_binary_buf(const Point *const points,
                              const size_t number_of_points) {
  if (number_of_points < 2) {
    lerp_error = NUMBER_OF_POINTS_BELOW_TWO;
    return default_lerp_binary_buf;
  }

  LerpBinaryBuf buf = default_lerp_binary_buf;
  const Point *const points_back_pointer = points + number_of_points - 1;
  buf.points_front_x = points->x;
  buf.points_back_x = points_back_pointer->x;
  buf.state = BINARY;
  buf.points_front_pointer = points;
  buf.points_back_pointer = points_back_pointer;
  return buf;
}

LerpError lerp_error = NONE;

FloatingPointType lerp_linear(LerpLinearBuf *const buf_pointer,
                              const FloatingPointType x) {
  const Point *points_end;
  const Point *points_iterator;
  FloatingPointType point_x;
  const Point *next_points_iterator;
  FloatingPointType next_point_x;
  FloatingPointType point_y;
  FloatingPointType m;
  const Point *points_begin;

/* With the pointer past the end of the points and the first iterator set,
 * verify that the first iterator does not point past the end of the points.
 */
#define CMP_FIRST_ITERATOR_AE                                                 \
  if (points_iterator >= points_end) {                                        \
    lerp_error = EXTRAPOLATION_ABOVE;                                         \
    return DEFAULT_FLOATING_POINT_TYPE;                                       \
  } //

/* With the first x-value set, verify that the given x-value is not less than
 * the first x-value.  Since the first x-value is the first point's x-value,
 * "interpolating" at an x-value lower than the first x-value would actually be
 * extrapolation.
 */
#define CMP_FIRST_X_B                                                         \
  if (x < point_x) {                                                          \
    lerp_error = EXTRAPOLATION_BELOW;                                         \
    return DEFAULT_FLOATING_POINT_TYPE;                                       \
  } //

/* With the first iterator and x-value set, get and then return the first
 * y-value.
 *
 * Before returning, write the first x-value and y-value and set the coroutine
 * to re-enter at LINEAR_Y_CMP_FIRST_X.
 */
#define RET_FIRST_Y                                                           \
  point_y = points_iterator->y;                                               \
  buf_pointer->state = LINEAR_Y_CMP_FIRST_X;                                  \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  return point_y

/* idem as CMP_FIRST_ITERATOR_AE, but for the first next iterator.
 */
#define CMP_FIRST_NEXT_ITERATOR_AE                                            \
  if (next_points_iterator >= points_end) {                                   \
    lerp_error = EXTRAPOLATION_ABOVE;                                         \
    return DEFAULT_FLOATING_POINT_TYPE;                                       \
  } //

/* With the first iterator, x-value, next iterator, and next x-value set, get
 * the first y-value and slope and then lerp.
 *
 * Before returning, write the first x-value, next iterator and x-value,
 * y-value, and slope and set the coroutine to re-enter at
 * LINEAR_M_CMP_FIRST_X.
 */
#define LERP_FIRST                                                            \
  point_y = points_iterator->y;                                               \
  m = get_m(point_x, next_points_iterator, next_point_x, point_y);            \
  buf_pointer->state = LINEAR_M_CMP_FIRST_X;                                  \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_x = next_point_x;                                   \
  buf_pointer->point_y = point_y;                                             \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

/* With the iterator and next x-value set, get and then return the y-value.
 * This should be run after the iterator has been set to the next iterator but
 * the x-value has not yet been set to the next x-value.  Thus, the next
 * x-value is actually the x-value.
 *
 * Before returning, write the iterator, x-value, and y-value and set the
 * coroutine to re-enter at LINEAR_Y_CMP_X.
 */
#define RET_Y_INC                                                             \
  point_y = points_iterator->y;                                               \
  buf_pointer->state = LINEAR_Y_CMP_X;                                        \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_x = next_point_x;                                        \
  buf_pointer->point_y = point_y;                                             \
  return point_y

/* With the pointer past the end of the points and the next iterator set,
 * verify that the next iterator does not point past the end of the points.
 */
#define CMP_NEXT_ITERATOR_AE                                                  \
  if (next_points_iterator >= points_end) {                                   \
    lerp_error = EXTRAPOLATION_ABOVE;                                         \
    return DEFAULT_FLOATING_POINT_TYPE;                                       \
  } //

/* With the iterator, x-value, next iterator, and next x-value set, get the
 * y-value and the slope and then lerp.
 *
 * Before returning, write the iterator, x-value, next iterator, next x-value,
 * y-value, and slope and set the coroutine to re-enter at LINEAR_M_CMP_X.
 */
#define LERP                                                                  \
  point_y = points_iterator->y;                                               \
  m = get_m(point_x, next_points_iterator, next_point_x, point_y);            \
  buf_pointer->state = LINEAR_M_CMP_X;                                        \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_x = next_point_x;                                   \
  buf_pointer->point_y = point_y;                                             \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

/* With the iterator and next x-value set, increment the iterator until the
 * given x-value is less than or equal to the next x-value and then lerp or
 * return the y-value respectively.  idem as for RET_Y_INC with the next
 * x-value.
 *
 * This macro always returns.
 */
#define INC_ITERATOR                                                          \
  point_x = next_point_x;                                                     \
                                                                              \
  for (;;) {                                                                  \
    next_points_iterator = points_iterator + 1;                               \
                                                                              \
    CMP_NEXT_ITERATOR_AE;                                                     \
                                                                              \
    next_point_x = next_points_iterator->x;                                   \
                                                                              \
    if (x < next_point_x) {                                                   \
      LERP;                                                                   \
    }                                                                         \
                                                                              \
    points_iterator = next_points_iterator;                                   \
                                                                              \
    if (x == next_point_x) {                                                  \
      RET_Y_INC;                                                              \
    }                                                                         \
                                                                              \
    point_x = next_point_x;                                                   \
  } //

/* With the first x-value, y-value, next iterator and x-value set, get the
 * slope and then lerp.
 *
 * Before returning, write the first next iterator and x-value and slope and
 * set the coroutine to re-enter at LINEAR_M_CMP_FIRST_X.
 */
#define Y_LERP_FIRST                                                          \
  m = get_m(point_x, next_points_iterator, next_point_x, point_y);            \
  buf_pointer->state = LINEAR_M_CMP_FIRST_X;                                  \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_x = next_point_x;                                   \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

/* With the pointer to the beginning of the points and the iterator set, verify
 * that the iterator does not point before the beginning of the points.
 */
#define CMP_ITERATOR_B                                                        \
  if (points_iterator < points_begin) {                                       \
    lerp_error = EXTRAPOLATION_BELOW;                                         \
    return DEFAULT_FLOATING_POINT_TYPE;                                       \
  } //

#define DEC_ITERATOR                                                          \
  CMP_ITERATOR_B;                                                             \
                                                                              \
  while (x < (point_x = points_iterator->x)) {                                \
    --points_iterator;                                                        \
                                                                              \
    CMP_ITERATOR_B;                                                           \
  } //

/* With the iterator and x-value set, get and then return the y-value.  This
 * should be run after the iterator has been decremented.
 *
 * Before returning, write the iterator, x-value, and y-value and set the
 * coroutine to re-enter at LINEAR_Y_CMP_X.
 */
#define RET_Y_DEC                                                             \
  point_y = points_iterator->y;                                               \
  buf_pointer->state = LINEAR_Y_CMP_X;                                        \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  return point_y

/* This should be run after the iterator has been decremented.
 */
#define GET_NEXT_X_DEC                                                        \
  /* It is safe to increment and then dereference the iterator because it has \
   * been decremented at least once.                                          \
   */                                                                         \
  next_points_iterator = points_iterator + 1;                                 \
  next_point_x = next_points_iterator->x

/* With the iterator, x-value, next iterator and x-value, and y-value set, get
 * the slope and then lerp.
 *
 * Before returning, write the next iterator and x-value and slope and set the
 * coroutine to re-enter at LINEAR_M_CMP_X.
 */
#define Y_LERP                                                                \
  m = get_m(point_x, next_points_iterator, next_point_x, point_y);            \
  buf_pointer->state = LINEAR_M_CMP_X;                                        \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_x = next_point_x;                                   \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

  switch (buf_pointer->state) {
  case LINEAR:
    points_end = buf_pointer->points_end;
    points_iterator = buf_pointer->points_iterator;

    CMP_FIRST_ITERATOR_AE;

    point_x = points_iterator->x;

    CMP_FIRST_X_B;

    if (x == point_x) {
      RET_FIRST_Y;
    }

    next_points_iterator = points_iterator + 1;

    CMP_FIRST_NEXT_ITERATOR_AE;

    next_point_x = next_points_iterator->x;

    if (x < next_point_x) {
      LERP_FIRST;
    }

    points_iterator = next_points_iterator;

    if (x == next_point_x) {
      RET_Y_INC;
    }

    INC_ITERATOR;
  case LINEAR_Y_CMP_FIRST_X:
    point_x = buf_pointer->point_x;

    CMP_FIRST_X_B;

    if (x == point_x)
      return buf_pointer->point_y;

    points_end = buf_pointer->points_end;
    next_points_iterator = buf_pointer->points_iterator + 1;

    CMP_FIRST_NEXT_ITERATOR_AE;

    next_point_x = next_points_iterator->x;

    if (x < next_point_x) {
      point_y = buf_pointer->point_y;
      Y_LERP_FIRST;
    }

    points_iterator = next_points_iterator;

    if (x == next_point_x) {
      RET_Y_INC;
    }

    INC_ITERATOR;
  case LINEAR_M_CMP_FIRST_X:
    point_x = buf_pointer->point_x;

    CMP_FIRST_X_B;

    if (x == point_x)
      return buf_pointer->point_y;

    next_point_x = buf_pointer->next_point_x;

    if (x < next_point_x)
      return lerp(buf_pointer->point_x, buf_pointer->point_y, buf_pointer->m,
                  x);

    points_iterator = buf_pointer->next_points_iterator;

    if (x == next_point_x) {
      RET_Y_INC;
    }

    points_end = buf_pointer->points_end;
    INC_ITERATOR;
  case LINEAR_Y_CMP_X:
    point_x = buf_pointer->point_x;

    if (x < point_x) {
      points_begin = buf_pointer->points_begin;
      points_iterator = buf_pointer->points_iterator - 1;

      DEC_ITERATOR;

      if (x == point_x) {
        RET_Y_DEC;
      }

      GET_NEXT_X_DEC;
      LERP;
    }

    if (x == point_x)
      return buf_pointer->point_y;

    points_end = buf_pointer->points_end;
    next_points_iterator = buf_pointer->points_iterator + 1;

    CMP_NEXT_ITERATOR_AE;

    next_point_x = next_points_iterator->x;

    if (x < next_point_x) {
      point_y = buf_pointer->point_y;
      Y_LERP;
    }

    points_iterator = next_points_iterator;

    if (x == next_point_x) {
      RET_Y_INC;
    }

    INC_ITERATOR;
  case LINEAR_M_CMP_X:
    point_x = buf_pointer->point_x;

    if (x < point_x) {
      points_begin = buf_pointer->points_begin;
      points_iterator = buf_pointer->points_iterator - 1;

      DEC_ITERATOR;

      if (x == point_x) {
        RET_Y_DEC;
      }

      GET_NEXT_X_DEC;
      LERP;
    }

    if (x == point_x)
      return buf_pointer->point_y;

    next_point_x = buf_pointer->next_point_x;

    if (x < next_point_x)
      return lerp(buf_pointer->point_x, buf_pointer->point_y, buf_pointer->m,
                  x);

    points_iterator = buf_pointer->next_points_iterator;

    if (x == next_point_x) {
      RET_Y_INC;
    }

    points_end = buf_pointer->points_end;
    INC_ITERATOR;
  }

#undef CMP_FIRST_ITERATOR_AE
#undef CMP_FIRST_X_B
#undef RET_FIRST_Y
#undef CMP_FIRST_NEXT_ITERATOR_AE
#undef LERP_FIRST
#undef RET_Y_INC
#undef CMP_NEXT_ITERATOR_AE
#undef LERP
#undef INC_ITERATOR
#undef Y_LERP_FIRST
#undef CMP_ITERATOR_B
#undef DEC_ITERATOR
#undef RET_Y_DEC
#undef GET_NEXT_X_DEC
#undef Y_LERP
}

FloatingPointType
lerp_z_linear(const LerpLinearBuf *const buf_pointer,
              const FloatingPointType x,
              FloatingPointType (*const get_z_pointer)(const Point *const)) {
  switch (buf_pointer->state) {
  case LINEAR:
    lerp_error = UNDEFINED;
    return DEFAULT_FLOATING_POINT_TYPE;
  case LINEAR_Y_CMP_FIRST_X:
  case LINEAR_Y_CMP_X:
    return get_z_pointer(buf_pointer->points_iterator);
  case LINEAR_M_CMP_FIRST_X:
  case LINEAR_M_CMP_X: {
    const FloatingPointType point_x = buf_pointer->point_x;
    const FloatingPointType point_z =
        get_z_pointer(buf_pointer->points_iterator);
    return lerp(point_x, point_z,
                get_z_m(point_x, buf_pointer->next_points_iterator, point_z,
                        get_z_pointer),
                x);
  }
  }
}

FloatingPointType lerp_binary(LerpBinaryBuf *const buf_pointer,
                              const FloatingPointType x) {
  if (x < buf_pointer->points_front_x) {
    lerp_error = EXTRAPOLATION_BELOW;
    return DEFAULT_FLOATING_POINT_TYPE;
  }

  if (x > buf_pointer->points_back_x) {
    lerp_error = EXTRAPOLATION_ABOVE;
    return DEFAULT_FLOATING_POINT_TYPE;
  }

#define GET_NUMBER_OF_EDGES                                                   \
  number_of_edges = points_back_pointer - points_front_pointer

#define LERP                                                                  \
  point_x = points_back_pointer->x;                                           \
  point_y = points_back_pointer->y;                                           \
  m = get_m(point_x, points_front_pointer, points_front_pointer->x, point_y); \
  buf_pointer->state = BINARY_B_M_CMP_X;                                      \
  buf_pointer->points_iterator = points_back_pointer;                         \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

#define GET_X                                                                 \
  points_iterator = points_front_pointer + number_of_edges / 2;               \
  point_x = points_iterator->x

#define RET_Y                                                                 \
  point_y = points_iterator->y;                                               \
  buf_pointer->state = BINARY_Y_CMP_X;                                        \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  return point_y

#define LERP_B                                                                \
  point_y = points_back_pointer->y;                                           \
  m = get_m(point_x, points_front_pointer, points_front_pointer->x, point_y); \
  buf_pointer->state = BINARY_B_M_CMP_X;                                      \
  buf_pointer->points_iterator = points_back_pointer;                         \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

#define LERP_A                                                                \
  point_y = points_front_pointer->y;                                          \
  m = get_m(point_x, points_back_pointer, points_back_pointer->x, point_y);   \
  buf_pointer->state = BINARY_A_M_CMP_X;                                      \
  buf_pointer->points_iterator = points_front_pointer;                        \
  buf_pointer->point_x = point_x;                                             \
  buf_pointer->point_y = point_y;                                             \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

#define NEXT_ITERATOR                                                         \
  for (;;) {                                                                  \
    GET_X;                                                                    \
                                                                              \
    if (x == point_x) {                                                       \
      RET_Y;                                                                  \
    }                                                                         \
                                                                              \
    if (x < point_x) {                                                        \
      points_back_pointer = points_iterator;                                  \
      GET_NUMBER_OF_EDGES;                                                    \
                                                                              \
      if (number_of_edges <= 1) {                                             \
        LERP_B;                                                               \
      }                                                                       \
                                                                              \
      continue;                                                               \
    }                                                                         \
                                                                              \
    points_front_pointer = points_iterator;                                   \
    GET_NUMBER_OF_EDGES;                                                      \
                                                                              \
    if (number_of_edges <= 1) {                                               \
      LERP_A;                                                                 \
    }                                                                         \
  } //

#define Y_LERP_B                                                              \
  point_y = buf_pointer->point_y;                                             \
  m = get_m(point_x, points_front_pointer, points_front_pointer->x, point_y); \
  buf_pointer->state = BINARY_B_M_CMP_X;                                      \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

#define Y_LERP_A                                                              \
  point_y = buf_pointer->point_y;                                             \
  m = get_m(point_x, points_back_pointer, points_back_pointer->x, point_y);   \
  buf_pointer->state = BINARY_A_M_CMP_X;                                      \
  buf_pointer->m = m;                                                         \
  return lerp(point_x, point_y, m, x)

#define B_M_RET_NEXT_Y                                                        \
  point_y = next_points_iterator->y;                                          \
  buf_pointer->state = BINARY_A_M_CMP_X;                                      \
  buf_pointer->points_iterator = next_points_iterator;                        \
  buf_pointer->point_x = next_point_x;                                        \
  buf_pointer->point_y = point_y;                                             \
  return point_y

#define A_M_RET_NEXT_Y                                                        \
  point_y = next_points_iterator->y;                                          \
  buf_pointer->state = BINARY_B_M_CMP_X;                                      \
  buf_pointer->points_iterator = next_points_iterator;                        \
  buf_pointer->point_x = next_point_x;                                        \
  buf_pointer->point_y = point_y;                                             \
  return point_y

  const Point *points_front_pointer;
  const Point *points_back_pointer;
  const Point *points_iterator;
  FloatingPointType point_x;
  FloatingPointType point_y;
  FloatingPointType m;

  size_t number_of_edges;
  const Point *next_points_iterator;
  FloatingPointType next_point_x;

  switch (buf_pointer->state) {
  case BINARY:
    points_front_pointer = buf_pointer->points_front_pointer;
    points_back_pointer = buf_pointer->points_back_pointer;
    GET_NUMBER_OF_EDGES;

    if (number_of_edges <= 1) {
      LERP;
    }

    NEXT_ITERATOR;
  case BINARY_Y_CMP_X:
    point_x = buf_pointer->point_x;

    if (x == point_x)
      return buf_pointer->point_y;

    if (x < point_x) {
      points_front_pointer = buf_pointer->points_front_pointer;
      points_back_pointer = buf_pointer->points_iterator;
      GET_NUMBER_OF_EDGES;

      if (number_of_edges <= 1) {
        Y_LERP_B;
      }

      NEXT_ITERATOR;
    }

    points_front_pointer = buf_pointer->points_iterator;
    points_back_pointer = buf_pointer->points_back_pointer;
    GET_NUMBER_OF_EDGES;

    if (number_of_edges <= 1) {
      Y_LERP_A;
    }

    NEXT_ITERATOR;
  case BINARY_B_M_CMP_X:
    point_x = buf_pointer->point_x;

    if (x == point_x)
      return buf_pointer->point_y;

    if (x < point_x) {
      next_points_iterator = buf_pointer->points_iterator - 1;
      next_point_x = next_points_iterator->x;

      if (x > next_point_x)
        return lerp(point_x, buf_pointer->point_y, buf_pointer->m, x);

      if (x == next_point_x) {
        B_M_RET_NEXT_Y;
      }

      points_front_pointer = buf_pointer->points_front_pointer;
      points_back_pointer = next_points_iterator;
      GET_NUMBER_OF_EDGES;

      if (number_of_edges <= 1) {
        point_x = points_back_pointer->x;
        LERP_B;
      }

      NEXT_ITERATOR;
    }

    points_front_pointer = buf_pointer->points_iterator;
    points_back_pointer = buf_pointer->points_back_pointer;
    GET_NUMBER_OF_EDGES;

    if (number_of_edges <= 1) {
      Y_LERP_A;
    }

    NEXT_ITERATOR;
  case BINARY_A_M_CMP_X:
    point_x = buf_pointer->point_x;

    if (x == point_x)
      return buf_pointer->point_y;

    if (x > point_x) {
      next_points_iterator = buf_pointer->points_iterator + 1;
      next_point_x = next_points_iterator->x;

      if (x < next_point_x)
        return lerp(point_x, buf_pointer->point_y, buf_pointer->m, x);

      if (x == next_point_x) {
        A_M_RET_NEXT_Y;
      }

      points_front_pointer = next_points_iterator;
      points_back_pointer = buf_pointer->points_back_pointer;
      GET_NUMBER_OF_EDGES;

      if (number_of_edges <= 1) {
        point_x = points_front_pointer->x;
        LERP_A;
      }

      NEXT_ITERATOR;
    }

    points_front_pointer = buf_pointer->points_front_pointer;
    points_back_pointer = buf_pointer->points_iterator;
    GET_NUMBER_OF_EDGES;

    if (number_of_edges <= 1) {
      Y_LERP_B;
    }

    NEXT_ITERATOR;
  }

#undef GET_NUMBER_OF_EDGES
#undef LERP
#undef GET_X
#undef RET_Y
#undef LERP_B
#undef LERP_A
#undef NEXT_ITERATOR
#undef Y_LERP_B
#undef Y_LERP_A
#undef B_M_RET_NEXT_Y
#undef A_M_RET_NEXT_Y
}

FloatingPointType
lerp_z_binary(const LerpBinaryBuf *const buf_pointer,
              const FloatingPointType x,
              FloatingPointType (*const get_z_pointer)(const Point *const)) {
  switch (buf_pointer->state) {
  case BINARY:
    lerp_error = UNDEFINED;
    return DEFAULT_FLOATING_POINT_TYPE;
  case BINARY_Y_CMP_X:
    return get_z_pointer(buf_pointer->points_iterator);
  case BINARY_B_M_CMP_X: {
    const FloatingPointType point_x = buf_pointer->point_x;
    const FloatingPointType point_z =
        get_z_pointer(buf_pointer->points_iterator);
    return lerp(point_x, point_z,
                get_z_m(point_x, buf_pointer->points_front_pointer, point_z,
                        get_z_pointer),
                x);
  }
  case BINARY_A_M_CMP_X: {
    const FloatingPointType point_x = buf_pointer->point_x;
    const FloatingPointType point_z =
        get_z_pointer(buf_pointer->points_iterator);
    return lerp(point_x, point_z,
                get_z_m(point_x, buf_pointer->points_back_pointer, point_z,
                        get_z_pointer),
                x);
  }
  }
}
