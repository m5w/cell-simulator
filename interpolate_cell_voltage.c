#include "interpolate_cell_voltage.h"

LerpCellVoltageType
lerp_cell_voltage_state_type(const CellDischargeCurvePoint *const points,
                             const size_t number_of_points) {
  const LerpCellVoltageType lerp_cell_voltage_state = {
      .state = LERP_CELL_VOLTAGE,
      .points_end = points + number_of_points,
      .points_iterator = points};
  return lerp_cell_voltage_state;
}

FloatingPointType
lerp_cell_voltage_linear_forward(LerpCellVoltageType *const state_pointer,
                                 const FloatingPointType charge) {
  switch (state_pointer->state) {
  case LERP_CELL_VOLTAGE:
    if (state_pointer->points_iterator >= state_pointer->points_end) {
      error = EXTRAPOLATION;
      return ERROR_FLOATING_POINT_TYPE;
    }

    state_pointer->point_charge =
        state_pointer->points_iterator->charge_discharged_from_cell;

    if (charge < state_pointer->point_charge) {
      error = EXTRAPOLATION_BELOW;
      return ERROR_FLOATING_POINT_TYPE;
    }

    if (charge == state_pointer->point_charge) {
      state_pointer->point_voltage =
          state_pointer->points_iterator->cell_voltage;
      state_pointer->state = CMP_CHARGE;
      return state_pointer->point_voltage;
    case CMP_CHARGE:
      if (charge == state_pointer->point_charge)
        return state_pointer->point_voltage;

/* CoMPare the given charge to the NEXT CHARGE: Below
 *
 * Get and bounds-check the next iterator, get the next charge, and, if the
 * given charge is less than the next charge, execute the following block.
 *
 * Note that, indeed, a block must follow this macro.
 *
 * This cannot be defined as an inline function because it may or may not
 * return.
 */
#define CMP_NEXT_CHARGE_B                                                      \
  state_pointer->next_points_iterator = state_pointer->points_iterator + 1;    \
                                                                               \
  if (state_pointer->next_points_iterator >= state_pointer->points_end) {      \
    error = EXTRAPOLATION_ABOVE;                                               \
    return ERROR_FLOATING_POINT_TYPE;                                          \
  }                                                                            \
                                                                               \
  state_pointer->next_point_charge =                                           \
      state_pointer->next_points_iterator->charge_discharged_from_cell;        \
                                                                               \
  if (charge < state_pointer->next_point_charge)

      CMP_NEXT_CHARGE_B {

        /* The voltage was set when the given charge equalled the point's
         * charge.
         */
        return lerp_cell_voltage_lerp(state_pointer, charge);
      }

/* CoMPare the given charge to the NEXT CHARGE: Equal
 *
 * Set the iterator to the next iterator and the charge to the next charge.  If
 * the given charge is equal to the next charge (which is also now the charge),
 * then get and return the voltage and set the coroutine to re-enter at
 * CMP_CHARGE.
 *
 * Like CMP_NEXT_CHARGE_B, this cannot be inlined because it may or may not
 * return.
 */
#define CMP_NEXT_CHARGE_E                                                      \
  state_pointer->point_charge = state_pointer->next_point_charge;              \
  state_pointer->points_iterator = state_pointer->next_points_iterator;        \
                                                                               \
  if (charge == state_pointer->point_charge) {                                 \
    state_pointer->point_voltage =                                             \
        state_pointer->points_iterator->cell_voltage;                          \
    state_pointer->state = CMP_CHARGE;                                         \
    return state_pointer->point_voltage;                                       \
  }

      CMP_NEXT_CHARGE_E
    }

    for (;;) {
      CMP_NEXT_CHARGE_B {

#undef CMP_NEXT_CHARGE_B

        state_pointer->point_voltage =
            state_pointer->points_iterator->cell_voltage;
        return lerp_cell_voltage_lerp(state_pointer, charge);

#undef LERP

      case CMP_NEXT_CHARGE:
        if (charge < state_pointer->next_point_charge)
          return lerp(state_pointer->point_charge, state_pointer->point_voltage,
                      state_pointer->m, charge);
      }

      CMP_NEXT_CHARGE_E

#undef CMP_NEXT_CHARGE_E
    }
  }
}

FloatingPointType lerp(const FloatingPointType x_1,
                       const FloatingPointType y_1, const FloatingPointType m,
                       const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}
