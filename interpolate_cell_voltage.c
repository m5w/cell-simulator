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
    }

    for (;;) {
      state_pointer->next_points_iterator = state_pointer->points_iterator + 1;

      if (state_pointer->next_points_iterator >= state_pointer->points_end) {
        error = EXTRAPOLATION_ABOVE;
        return ERROR_FLOATING_POINT_TYPE;
      }

      state_pointer->next_point_charge =
          state_pointer->next_points_iterator->charge_discharged_from_cell;

      if (charge < state_pointer->next_point_charge) {
        state_pointer->point_voltage =
            state_pointer->points_iterator->cell_voltage;
        state_pointer->m =
            (state_pointer->next_points_iterator->cell_voltage -
             state_pointer->point_voltage) /
            (state_pointer->next_point_charge - state_pointer->point_charge);
        state_pointer->state = CMP_NEXT_POINT_CHARGE;
        return lerp(state_pointer->point_charge, state_pointer->point_voltage,
                    state_pointer->m, charge);
      case CMP_NEXT_POINT_CHARGE:
        if (charge < state_pointer->next_point_charge)
          return lerp(state_pointer->point_charge, state_pointer->point_voltage,
                      state_pointer->m, charge);
      }

      state_pointer->point_charge = state_pointer->next_point_charge;
      state_pointer->points_iterator = state_pointer->next_points_iterator;

      if (charge == state_pointer->point_charge) {
        state_pointer->point_voltage =
            state_pointer->points_iterator->cell_voltage;
        state_pointer->state = CMP_POINT_CHARGE;
        return state_pointer->point_voltage;
      }
    }
  }
}

static FloatingPointType lerp(const FloatingPointType x_1,
                              const FloatingPointType y_1,
                              const FloatingPointType m,
                              const FloatingPointType x) {
  return y_1 + (x - x_1) * m;
}
