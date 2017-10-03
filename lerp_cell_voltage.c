#include "lerp_cell_voltage.h"

LerpCellVoltageLinearBufType
lerp_cell_voltage_linear_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points) {
  if (number_of_points < 2) {
    error = EXTRAPOLATION;
    return ERROR_LERP_CELL_VOLTAGE_LINEAR_BUF_TYPE;
  }

  const LerpCellVoltageLinearBufType buf = {.state = LINEAR,
                                            .points_begin = points,
                                            .points_end =
                                                points + number_of_points,
                                            .points_iterator = points};
  return buf;
}

LerpCellVoltageBinaryBufType
lerp_cell_voltage_binary_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points) {
  if (number_of_points < 2) {
    error = EXTRAPOLATION;
    return ERROR_LERP_CELL_VOLTAGE_BINARY_BUF_TYPE;
  }

  const CellDischargeCurvePoint *const points_back_pointer =
      points + number_of_points - 1;
  const LerpCellVoltageBinaryBufType buf = {
      .points_front_charge = points->charge_discharged_from_cell,
      .points_back_charge = points_back_pointer->charge_discharged_from_cell,
      .state = BINARY,
      .points_front_pointer = points,
      .points_back_pointer = points_back_pointer};
  return buf;
}

FloatingPointType
lerp_cell_voltage_linear(LerpCellVoltageLinearBufType *const buf_pointer,
                         const FloatingPointType charge) {
  const CellDischargeCurvePoint *points_begin;
  const CellDischargeCurvePoint *points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;

/* With the pointer-past-the-end of the cell discharge curve and the first
 * iterator set, verify that the first iterator does not point past the end of
 * the curve.
 */
#define CMP_FIRST_ITERATOR_AE                                                 \
  if (points_iterator >= points_end) {                                        \
    error = EXTRAPOLATION_ABOVE;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the first amount of charge discharged from the cell set, verify that
 * the given charge is not less than the first charge.  Since the first charge
 * is the lowest charge in the cell discharge curve, "interpolating" at a
 * charge lower than the first charge would actually be extrapolation.
 */
#define CMP_FIRST_CHARGE_B                                                    \
  if (charge < point_charge) {                                                \
    error = EXTRAPOLATION_BELOW;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the first iterator and amount of charge discharged from the cell set,
 * get and then return the first voltage across the cell's voltage source.
 *
 * Before returning, write the first charge and voltage and set the coroutine
 * to re-enter at LINEAR_VOLTAGE_CMP_FIRST_CHARGE.
 */
#define RET_FIRST_VOLTAGE                                                     \
  point_voltage = points_iterator->cell_voltage;                              \
  buf_pointer->state = LINEAR_VOLTAGE_CMP_FIRST_CHARGE;                       \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

/* idem as CMP_FIRST_ITERATOR_AE, but for the first next iterator.
 */
#define CMP_FIRST_NEXT_ITERATOR_AE                                            \
  if (next_points_iterator >= points_end) {                                   \
    error = EXTRAPOLATION_ABOVE;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the first iterator, amount of charge discharged from the cell, next
 * iterator, and next charge set, get the first voltage across the cell's
 * voltage source and slope and then lerp.
 *
 * Before returning, write the first charge, next iterator and charge, voltage,
 * and slope and set the coroutine to re-enter at LINEAR_M_CMP_FIRST_CHARGE.
 */
#define LERP_FIRST                                                            \
  point_voltage = points_iterator->cell_voltage;                              \
  m = lerp_cell_voltage_get_m(point_charge, next_points_iterator,             \
                              next_point_charge, point_voltage);              \
  buf_pointer->state = LINEAR_M_CMP_FIRST_CHARGE;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_charge = next_point_charge;                         \
  buf_pointer->point_voltage = point_voltage;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

/* With the iterator and next amount of charge discharged from the cell set,
 * get and then return the voltage across the cell's voltage source.  This
 * should be run after the iterator has been set to the next iterator but the
 * charge has not yet been set to the next charge.  Thus, the next charge is
 * actually the charge.
 *
 * Before returning, write the iterator, charge, and voltage and set the
 * coroutine to re-enter at LINEAR_VOLTAGE_CMP_CHARGE.
 */
#define RET_VOLTAGE_INC                                                       \
  point_voltage = points_iterator->cell_voltage;                              \
  buf_pointer->state = LINEAR_VOLTAGE_CMP_CHARGE;                             \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = next_point_charge;                              \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

/* With the pointer-past-the-end of the cell discharge curve and the next
 * iterator set, verify that the next iterator does not point past the end of
 * the curve.
 */
#define CMP_NEXT_ITERATOR_AE                                                  \
  if (next_points_iterator >= points_end) {                                   \
    error = EXTRAPOLATION_ABOVE;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the iterator, amound of charge discharged from the cell, next iterator,
 * and next charge set, get the voltage across the cell's voltage source and
 * the slope and then lerp.
 *
 * Before returning, write the iterator, charge, next iterator, next charge,
 * voltage, and slope and set the coroutine to re-enter at LINEAR_M_CMP_CHARGE.
 */
#define LERP                                                                  \
  point_voltage = points_iterator->cell_voltage;                              \
  m = lerp_cell_voltage_get_m(point_charge, next_points_iterator,             \
                              next_point_charge, point_voltage);              \
  buf_pointer->state = LINEAR_M_CMP_CHARGE;                                   \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_charge = next_point_charge;                         \
  buf_pointer->point_voltage = point_voltage;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

/* With the iterator and next amount of charge discharged from the cell set,
 * increment the iterator until the given charge is less than or equal to the
 * next charge and then lerp or return the voltage across the cell's voltage
 * source respectively.  idem as for RET_VOLTAGE_INC with the next charge.
 * This macro always returns.
 */
#define INC_ITERATOR                                                          \
  point_charge = next_point_charge;                                           \
                                                                              \
  for (;;) {                                                                  \
    next_points_iterator = points_iterator + 1;                               \
                                                                              \
    CMP_NEXT_ITERATOR_AE;                                                     \
                                                                              \
    next_point_charge = next_points_iterator->charge_discharged_from_cell;    \
                                                                              \
    if (charge < next_point_charge) {                                         \
      LERP;                                                                   \
    }                                                                         \
                                                                              \
    points_iterator = next_points_iterator;                                   \
                                                                              \
    if (charge == next_point_charge) {                                        \
      RET_VOLTAGE_INC;                                                        \
    }                                                                         \
                                                                              \
    point_charge = next_point_charge;                                         \
  } //

/* With the first charge, voltage, next iterator, and next charge set, get the
 * slope and then lerp.
 *
 * Before returning write the first next iterator, next charge, and slope and
 * set the coroutine to re-enter at LINEAR_M_CMP_FIRST_CHARGE.
 */
#define VOLTAGE_LERP_FIRST                                                    \
  m = lerp_cell_voltage_get_m(point_charge, next_points_iterator,             \
                              next_point_charge, point_voltage);              \
  buf_pointer->state = LINEAR_M_CMP_FIRST_CHARGE;                             \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_charge = next_point_charge;                         \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

/* With the pointer-to-the-beginning of the curve and the iterator set, verify
 * that the iterator does not point before the start of the curve.
 */
#define CMP_ITERATOR_B                                                        \
  if (points_iterator < points_begin) {                                       \
    error = EXTRAPOLATION_BELOW;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

#define DEC_ITERATOR                                                          \
  CMP_ITERATOR_B;                                                             \
                                                                              \
  while (charge <                                                             \
         (point_charge = points_iterator->charge_discharged_from_cell)) {     \
    --points_iterator;                                                        \
                                                                              \
    CMP_ITERATOR_B;                                                           \
  } //

#define RET_VOLTAGE_DEC                                                       \
  point_voltage = points_iterator->cell_voltage;                              \
  buf_pointer->state = LINEAR_VOLTAGE_CMP_CHARGE;                             \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

#define VOLTAGE_LERP                                                          \
  m = lerp_cell_voltage_get_m(point_charge, next_points_iterator,             \
                              next_point_charge, point_voltage);              \
  buf_pointer->state = LINEAR_M_CMP_CHARGE;                                   \
  buf_pointer->next_points_iterator = next_points_iterator;                   \
  buf_pointer->next_point_charge = next_point_charge;                         \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

  switch (buf_pointer->state) {
  case LINEAR:
    points_end = buf_pointer->points_end;
    points_iterator = buf_pointer->points_iterator;

    CMP_FIRST_ITERATOR_AE;

    point_charge = points_iterator->charge_discharged_from_cell;

    CMP_FIRST_CHARGE_B;

    if (charge == point_charge) {
      RET_FIRST_VOLTAGE;
    }

    next_points_iterator = points_iterator + 1;

    CMP_FIRST_NEXT_ITERATOR_AE;

    next_point_charge = next_points_iterator->charge_discharged_from_cell;

    if (charge < next_point_charge) {
      LERP_FIRST;
    }

    points_iterator = next_points_iterator;

    if (charge == next_point_charge) {
      RET_VOLTAGE_INC;
    }

    INC_ITERATOR;
  case LINEAR_VOLTAGE_CMP_FIRST_CHARGE:
    point_charge = buf_pointer->point_charge;

    CMP_FIRST_CHARGE_B;

    if (charge == point_charge)
      return buf_pointer->point_voltage;

    points_end = buf_pointer->points_end;
    next_points_iterator = buf_pointer->points_iterator + 1;

    CMP_FIRST_NEXT_ITERATOR_AE;

    next_point_charge = next_points_iterator->charge_discharged_from_cell;

    if (charge < next_point_charge) {
      point_voltage = buf_pointer->point_voltage;
      VOLTAGE_LERP_FIRST;
    case LINEAR_M_CMP_FIRST_CHARGE:
      point_charge = buf_pointer->point_charge;

      CMP_FIRST_CHARGE_B;

      if (charge == point_charge)
        return buf_pointer->point_charge;

      next_point_charge = buf_pointer->next_point_charge;

      if (charge < next_point_charge)
        return lerp(buf_pointer->point_charge, buf_pointer->point_voltage,
                    buf_pointer->m, charge);

      points_iterator = buf_pointer->next_points_iterator;

      if (charge == next_point_charge) {
        RET_VOLTAGE_INC;
      case LINEAR_VOLTAGE_CMP_CHARGE:
        point_charge = buf_pointer->point_charge;

        if (charge < point_charge) {
          points_begin = buf_pointer->points_begin;
          points_iterator = buf_pointer->points_iterator - 1;

          DEC_ITERATOR;

          if (charge == point_charge) {
            RET_VOLTAGE_DEC;
          }

          /* It is safe to increment and then dereference the iterator
           * because it was decremented at least once.
           */
          next_points_iterator = points_iterator + 1;
          next_point_charge =
              next_points_iterator->charge_discharged_from_cell;

          LERP;
        case LINEAR_M_CMP_CHARGE:
          point_charge = buf_pointer->point_charge;

          if (charge < point_charge) {
            points_begin = buf_pointer->points_begin;
            points_iterator = buf_pointer->points_iterator - 1;

            DEC_ITERATOR;

            if (charge == point_charge) {
              RET_VOLTAGE_DEC;
            }

            /* It is safe to increment and then dereference the iterator
             * because it was decremented at least once.
             */
            next_points_iterator = points_iterator + 1;
            next_point_charge =
                next_points_iterator->charge_discharged_from_cell;

            LERP;
          }

          if (charge == point_charge)
            return buf_pointer->point_voltage;

          next_point_charge = buf_pointer->next_point_charge;

          if (charge < next_point_charge)
            return lerp(buf_pointer->point_charge, buf_pointer->point_voltage,
                        buf_pointer->m, charge);

          points_iterator = buf_pointer->next_points_iterator;

          if (charge == next_point_charge) {
            RET_VOLTAGE_INC;
          }

          INC_ITERATOR;
        }

        if (charge == point_charge)
          return buf_pointer->point_voltage;

        points_end = buf_pointer->points_end;
        next_points_iterator = buf_pointer->points_iterator + 1;

        CMP_NEXT_ITERATOR_AE;

        next_point_charge = next_points_iterator->charge_discharged_from_cell;

        if (charge < next_point_charge) {
          point_voltage = buf_pointer->point_voltage;
          VOLTAGE_LERP;
        }

        points_iterator = next_points_iterator;

        if (charge == next_point_charge) {
          RET_VOLTAGE_INC;
        }

        INC_ITERATOR;
      }

      INC_ITERATOR;
    }

    points_iterator = next_points_iterator;

    if (charge == next_point_charge) {
      RET_VOLTAGE_INC;
    }

    INC_ITERATOR;
  }

#undef CMP_FIRST_ITERATOR_AE
#undef CMP_FIRST_CHARGE_B
#undef RET_FIRST_VOLTAGE
#undef CMP_FIRST_NEXT_ITERATOR_AE
#undef VOLTAGE_LERP_FIRST
#undef RET_VOLTAGE_INC
#undef CMP_ITERATOR_B
#undef DEC_ITERATOR
#undef RET_VOLTAGE_DEC
#undef LERP
#undef CMP_NEXT_ITERATOR_AE
#undef VOLTAGE_LERP
#undef INC_ITERATOR
#undef LERP_FIRST
}

FloatingPointType
lerp_cell_voltage_binary(LerpCellVoltageBinaryBufType *const buf_pointer,
                         const FloatingPointType charge) {
  if (charge < buf_pointer->points_front_charge) {
    error = EXTRAPOLATION_BELOW;
    return ERROR_FLOATING_POINT_TYPE;
  }

  if (charge > buf_pointer->points_back_charge) {
    error = EXTRAPOLATION_ABOVE;
    return ERROR_FLOATING_POINT_TYPE;
  }

#define GET_NUMBER_OF_EDGES                                                   \
  number_of_edges = points_back_pointer - points_front_pointer

#define LERP                                                                  \
  point_charge = points_back_pointer->charge_discharged_from_cell;            \
  point_voltage = points_back_pointer->cell_voltage;                          \
  m = lerp_cell_voltage_get_m(                                                \
      point_charge, points_front_pointer,                                     \
      points_front_pointer->charge_discharged_from_cell, point_voltage);      \
  buf_pointer->state = BINARY_B_M_CMP_CHARGE;                                 \
  buf_pointer->points_iterator = points_back_pointer;                         \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

#define GET_CHARGE                                                            \
  points_iterator = points_front_pointer + number_of_edges / 2;               \
  point_charge = points_iterator->charge_discharged_from_cell

#define RET_VOLTAGE                                                           \
  point_voltage = points_iterator->cell_voltage;                              \
  buf_pointer->state = BINARY_VOLTAGE_CMP_CHARGE;                             \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

#define VOLTAGE_CMP_CHARGE_E                                                  \
  if (charge == point_charge)                                                 \
  return buf_pointer->point_voltage

#define VOLTAGE_LERP_B                                                        \
  point_voltage = buf_pointer->point_voltage;                                 \
  m = lerp_cell_voltage_get_m(                                                \
      point_charge, points_front_pointer,                                     \
      points_front_pointer->charge_discharged_from_cell, point_voltage);      \
  buf_pointer->state = BINARY_B_M_CMP_CHARGE;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

#define LERP_B                                                                \
  point_voltage = points_iterator->cell_voltage;                              \
  m = lerp_cell_voltage_get_m(                                                \
      point_charge, points_front_pointer,                                     \
      points_front_pointer->charge_discharged_from_cell, point_voltage);      \
  buf_pointer->state = BINARY_B_M_CMP_CHARGE;                                 \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

#define B_M_CMP_NEXT_CHARGE_A                                                 \
  if (charge > next_point_charge)                                             \
  return lerp(point_charge, buf_pointer->point_voltage, buf_pointer->m, charge)

#define B_M_RET_NEXT_VOLTAGE                                                  \
  point_voltage = next_points_iterator->cell_voltage;                         \
  buf_pointer->state = BINARY_A_M_CMP_CHARGE;                                 \
  buf_pointer->points_iterator = next_points_iterator;                        \
  buf_pointer->point_charge = next_point_charge;                              \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

#define A_M_CMP_NEXT_CHARGE_B                                                 \
  if (charge < next_point_charge)                                             \
  return lerp(point_charge, buf_pointer->point_voltage, buf_pointer->m, charge)

#define A_M_RET_NEXT_VOLTAGE                                                  \
  point_voltage = next_points_iterator->cell_voltage;                         \
  buf_pointer->state = BINARY_B_M_CMP_CHARGE;                                 \
  buf_pointer->points_iterator = next_points_iterator;                        \
  buf_pointer->point_charge = next_point_charge;                              \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

#define LERP_A                                                                \
  point_voltage = points_iterator->cell_voltage;                              \
  m = lerp_cell_voltage_get_m(                                                \
      point_charge, points_back_pointer,                                      \
      points_back_pointer->charge_discharged_from_cell, point_voltage);       \
  buf_pointer->state = BINARY_A_M_CMP_CHARGE;                                 \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = point_charge;                                   \
  buf_pointer->point_voltage = point_voltage;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

#define NEXT_ITERATOR                                                         \
  for (;;) {                                                                  \
    GET_CHARGE;                                                               \
                                                                              \
    if (charge == point_charge) {                                             \
      RET_VOLTAGE;                                                            \
    }                                                                         \
                                                                              \
    if (charge < point_charge) {                                              \
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

#define VOLTAGE_LERP_A                                                        \
  point_voltage = buf_pointer->point_voltage;                                 \
  m = lerp_cell_voltage_get_m(                                                \
      point_charge, points_back_pointer,                                      \
      points_back_pointer->charge_discharged_from_cell, point_voltage);       \
  buf_pointer->state = BINARY_A_M_CMP_CHARGE;                                 \
  buf_pointer->m = m;                                                         \
  return lerp(point_charge, point_voltage, m, charge)

  const CellDischargeCurvePoint *points_front_pointer;
  const CellDischargeCurvePoint *points_back_pointer;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;

  size_t number_of_edges;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;

  switch (buf_pointer->state) {
  case BINARY:
    points_front_pointer = buf_pointer->points_front_pointer;
    points_back_pointer = buf_pointer->points_back_pointer;
    GET_NUMBER_OF_EDGES;

    if (number_of_edges <= 1) {
      LERP;
    }

    for (;;) {
      GET_CHARGE;

      if (charge == point_charge) {
        RET_VOLTAGE;
      case BINARY_VOLTAGE_CMP_CHARGE:
        point_charge = buf_pointer->point_charge;

        VOLTAGE_CMP_CHARGE_E;

        if (charge < point_charge) {
          points_front_pointer = buf_pointer->points_front_pointer;
          points_back_pointer = buf_pointer->points_iterator;
          GET_NUMBER_OF_EDGES;

          if (number_of_edges <= 1) {
            VOLTAGE_LERP_B;
          }

          for (;;) {
            GET_CHARGE;

            if (charge == point_charge) {
              RET_VOLTAGE;
            }

            if (charge < point_charge) {
              points_back_pointer = points_iterator;
              GET_NUMBER_OF_EDGES;

              if (number_of_edges <= 1) {
                LERP_B;
              case BINARY_B_M_CMP_CHARGE:
                point_charge = buf_pointer->point_charge;

                VOLTAGE_CMP_CHARGE_E;

                if (charge < point_charge) {
                  next_points_iterator = buf_pointer->points_iterator - 1;
                  next_point_charge =
                      next_points_iterator->charge_discharged_from_cell;

                  B_M_CMP_NEXT_CHARGE_A;

                  if (charge == next_point_charge) {
                    B_M_RET_NEXT_VOLTAGE;
                  case BINARY_A_M_CMP_CHARGE:
                    point_charge = buf_pointer->point_charge;

                    VOLTAGE_CMP_CHARGE_E;

                    if (charge > point_charge) {
                      next_points_iterator = buf_pointer->points_iterator + 1;
                      next_point_charge =
                          next_points_iterator->charge_discharged_from_cell;

                      A_M_CMP_NEXT_CHARGE_B;

                      if (charge == next_point_charge) {
                        A_M_RET_NEXT_VOLTAGE;
                      }

                      points_front_pointer = next_points_iterator;
                      points_back_pointer = buf_pointer->points_back_pointer;
                      GET_NUMBER_OF_EDGES;

                      if (number_of_edges <= 1) {
                        LERP_A;
                      }

                      NEXT_ITERATOR;
                    }

                    points_front_pointer = buf_pointer->points_front_pointer;
                    points_back_pointer = buf_pointer->points_iterator;
                    GET_NUMBER_OF_EDGES;

                    if (number_of_edges <= 1) {
                      VOLTAGE_LERP_B;
                    }

                    NEXT_ITERATOR;
                  }

                  points_front_pointer = buf_pointer->points_front_pointer;
                  points_back_pointer = next_points_iterator;
                  GET_NUMBER_OF_EDGES;

                  if (number_of_edges <= 1) {
                    LERP_B;
                  }

                  NEXT_ITERATOR;
                }

                points_front_pointer = buf_pointer->points_iterator;
                points_back_pointer = buf_pointer->points_back_pointer;
                GET_NUMBER_OF_EDGES;

                if (number_of_edges <= 1) {
                  VOLTAGE_LERP_A;
                }

                NEXT_ITERATOR;
              }

              continue;
            }

            points_front_pointer = points_iterator;
            GET_NUMBER_OF_EDGES;

            if (number_of_edges <= 1) {
              LERP_A;
            }
          }
        }

        points_front_pointer = buf_pointer->points_iterator;
        points_back_pointer = buf_pointer->points_back_pointer;
        GET_NUMBER_OF_EDGES;

        if (number_of_edges <= 1) {
          VOLTAGE_LERP_A;
        }

        NEXT_ITERATOR;
      }

      if (charge < point_charge) {
        points_back_pointer = points_iterator;
        GET_NUMBER_OF_EDGES;

        if (number_of_edges <= 1) {
          LERP_B;
        }

        continue;
      }

      points_front_pointer = points_iterator;
      GET_NUMBER_OF_EDGES;

      if (number_of_edges <= 1) {
        LERP_A;
      }
    }
  }

#undef GET_NUMBER_OF_EDGES
#undef LERP
#undef GET_CHARGE
#undef RET_VOLTAGE
#undef VOLTAGE_CMP_CHARGE_E
#undef VOLTAGE_LERP_B
#undef LERP_B
#undef B_M_CMP_NEXT_CHARGE_A
#undef B_M_RET_NEXT_VOLTAGE
#undef A_M_CMP_NEXT_CHARGE_B
#undef A_M_RET_NEXT_VOLTAGE
#undef LERP_A
#undef NEXT_ITERATOR
#undef VOLTAGE_LERP_A
}

FloatingPointType lerp(const FloatingPointType x_1,
                       const FloatingPointType y_1, const FloatingPointType m,
                       const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}
