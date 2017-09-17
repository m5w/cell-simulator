#include "lerp_cell_voltage.h"

LerpCellVoltageLinearForwardBufType lerp_cell_voltage_linear_forward_buf_type(
    const CellDischargeCurvePoint *const points,
    const size_t number_of_points) {
  const LerpCellVoltageLinearForwardBufType buf = {
      .state = LINEAR_FORWARD,
      .points_end = points + number_of_points,
      .points_iterator = points};
  return buf;
}

LerpCellVoltageLinearBufType
lerp_cell_voltage_linear_buf_type(const CellDischargeCurvePoint *const points,
                                  const size_t number_of_points) {
  const LerpCellVoltageLinearBufType buf = {.state = LINEAR,
                                            .points_begin = points,
                                            .points_end =
                                                points + number_of_points,
                                            .points_iterator = points};
  return buf;
}

FloatingPointType lerp_cell_voltage_linear_forward(
    LerpCellVoltageLinearForwardBufType *const buf_pointer,
    const FloatingPointType charge) {
  const CellDischargeCurvePoint *points_end;
  const CellDischargeCurvePoint *points_iterator;
  FloatingPointType point_charge;
  const CellDischargeCurvePoint *next_points_iterator;
  FloatingPointType next_point_charge;
  FloatingPointType point_voltage;
  FloatingPointType m;
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

/* With the pointer-past-the-end of the curve and the first iterator set,
 * verify that the first iterator does not point past the end of the curve.
 */
#define CMP_FIRST_ITERATOR_AE                                                 \
  if (points_iterator >= points_end) {                                        \
    error = EXTRAPOLATION;                                                    \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the first charge set, verify that the given charge is not less than the
 * first charge.  Since the first charge is the lowest charge in the curve,
 * interpolating at a charge lower than the first charge would be
 * extrapolation.
 */
#define CMP_FIRST_CHARGE_B                                                    \
  if (charge < point_charge) {                                                \
    error = EXTRAPOLATION_BELOW;                                              \
    return ERROR_FLOATING_POINT_TYPE;                                         \
  } //

/* With the first iterator and charge set, get and then return the first
 * voltage.
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

/* With the iterator and next charge set, get and then return the voltage.
 *
 * Before returning, write the iterator, next charge, and voltage and set the
 * coroutine to re-enter at LINEAR_VOLTAGE_CMP_CHARGE.
 */
#define RET_VOLTAGE_INC                                                       \
  point_voltage = points_iterator->cell_voltage;                              \
  buf_pointer->state = LINEAR_VOLTAGE_CMP_CHARGE;                             \
  buf_pointer->points_iterator = points_iterator;                             \
  buf_pointer->point_charge = next_point_charge;                              \
  buf_pointer->point_voltage = point_voltage;                                 \
  return point_voltage

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

#define LERP                                                                  \
  point_voltage = points_iterator->cell_voltage;                              \
                                                                              \
  /* It is safe to increment and then dereference the iterator because it was \
   * decremented at least once.                                               \
   */                                                                         \
  next_points_iterator = points_iterator + 1;                                 \
  next_point_charge = next_points_iterator->charge_discharged_from_cell;      \
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

  switch (buf_pointer->state) {
  case LINEAR:
    points_end = buf_pointer->points_end;
    points_iterator = buf_pointer->points_iterator;

    CMP_FIRST_ITERATOR_AE;

    point_charge = points_iterator->charge_discharged_from_cell;

    CMP_FIRST_CHARGE_B;

    if (charge == point_charge) {
      RET_FIRST_VOLTAGE;
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

              LERP;
            }

            if (charge == point_charge)
              return buf_pointer->point_voltage;
          }
        }
      }
    }
  }
}

FloatingPointType lerp_cell_voltage_get_m(
    const FloatingPointType point_charge,
    const CellDischargeCurvePoint *const next_points_iterator,
    const FloatingPointType next_point_charge,
    const FloatingPointType point_voltage) {
  return (next_points_iterator->cell_voltage - point_voltage) /
         (next_point_charge - point_charge);
}

FloatingPointType lerp(const FloatingPointType x_1,
                       const FloatingPointType y_1, const FloatingPointType m,
                       const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}
