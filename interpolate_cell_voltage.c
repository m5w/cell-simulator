#include "interpolate_cell_voltage.h"

LerpCellVoltageStateType
lerp_cell_voltage_state_type(const CellDischargeCurvePoint *const points,
                             const size_t number_of_points) {
  const LerpCellVoltageStateType lerp_cell_voltage_state = {
      .state = LERP_CELL_VOLTAGE,
      .points_end = points + number_of_points,
      .points_iterator = points};
  return lerp_cell_voltage_state;
}

FloatingPointType
lerp_cell_voltage(LerpCellVoltageStateType *const state_pointer,
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
      state_pointer->state = CMP_POINT_CHARGE;
      return state_pointer->point_voltage;
    case CMP_POINT_CHARGE:
      if (charge == state_pointer->point_charge)
        return state_pointer->point_voltage;

#define FOR_BEGIN                                                              \
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
  if (charge < state_pointer->next_point_charge) {

      FOR_BEGIN

/* _L_inear int_ERP_olation
 *
 * Assuming that the voltage is set, set the slope and linearly interpolate the
 * voltage between the point and the next point at the given charge.  Set the
 * coroutine to re-enter at CMP_NEXT_POINT_CHARGE.
 */
#define LERP                                                                   \
  state_pointer->m =                                                           \
      (state_pointer->next_points_iterator->cell_voltage -                     \
       state_pointer->point_voltage) /                                         \
      (state_pointer->next_point_charge - state_pointer->point_charge);        \
  state_pointer->state = CMP_NEXT_POINT_CHARGE;                                \
  return lerp(state_pointer->point_charge, state_pointer->point_voltage,       \
              state_pointer->m, charge);

      /* The voltage was set when the given charge equalled the point's charge.
       * Using these macros allows the computer to skip repeating the
       * assignment.
       */
      LERP

#define FOR_END                                                                \
  }                                                                            \
                                                                               \
  state_pointer->point_charge = state_pointer->next_point_charge;              \
  state_pointer->points_iterator = state_pointer->next_points_iterator;        \
                                                                               \
  if (charge == state_pointer->point_charge) {                                 \
    state_pointer->point_voltage =                                             \
        state_pointer->points_iterator->cell_voltage;                          \
    state_pointer->state = CMP_POINT_CHARGE;                                   \
    return state_pointer->point_voltage;                                       \
  }

          FOR_END
    }

    for (;;) {
      FOR_BEGIN

#undef FOR_BEGIN

      state_pointer->point_voltage =
          state_pointer->points_iterator->cell_voltage;
      LERP

#undef LERP

          case CMP_NEXT_POINT_CHARGE
          : if (charge < state_pointer->next_point_charge) return lerp(
                state_pointer->point_charge, state_pointer->point_voltage,
                state_pointer->m, charge);
      FOR_END

#undef FOR_END
    }
  }
}

static FloatingPointType lerp(const FloatingPointType x_1,
                              const FloatingPointType y_1,
                              const FloatingPointType m,
                              const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}
