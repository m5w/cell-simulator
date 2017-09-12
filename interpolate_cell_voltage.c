#include "interpolate_cell_voltage.h"

FloatingPointType
interpolate_cell_voltage(const CellDischargeCurvePoint *const points,
                         const size_t number_of_points,
                         const FloatingPointType charge) {
  static enum {
    INTERPOLATE_CELL_VOLTAGE,
    CMP_POINT_CHARGE,
    CMP_NEXT_POINT_CHARGE
  } state = INTERPOLATE_CELL_VOLTAGE;
  static const CellDischargeCurvePoint *points_end;
  static const CellDischargeCurvePoint *points_iterator;
  static FloatingPointType point_charge;
  static const CellDischargeCurvePoint *next_points_iterator;
  static FloatingPointType next_point_charge;
  static FloatingPointType point_voltage;
  static FloatingPointType slope;

  switch (state) {
  case INTERPOLATE_CELL_VOLTAGE:
    points_end = points + number_of_points;
    points_iterator = points;

    if (points_iterator >= points_end) {
      error = EXTRAPOLATION;
      return ERROR_FLOATING_POINT_TYPE;
    }

    point_charge = points_iterator->charge_discharged_from_cell;

    if (charge < point_charge) {
      error = EXTRAPOLATION_BELOW;
      return ERROR_FLOATING_POINT_TYPE;
    }
    if (charge == point_charge) {
      point_voltage = points_iterator->cell_voltage;
      state = CMP_POINT_CHARGE;
      return point_voltage;
    case CMP_POINT_CHARGE:
      if (charge == point_charge)
        return point_voltage;
    }

    for (;;) {
      next_points_iterator = points_iterator + 1;

      if (next_points_iterator >= points_end) {
        error = EXTRAPOLATION_ABOVE;
        return ERROR_FLOATING_POINT_TYPE;
      }

      next_point_charge = next_points_iterator->charge_discharged_from_cell;

      if (charge < next_point_charge) {
        point_voltage = points_iterator->cell_voltage;
        slope = (next_points_iterator->cell_voltage - point_voltage) /
                (next_point_charge - point_charge);
        state = CMP_NEXT_POINT_CHARGE;
        return interpolate(point_charge, point_voltage, slope, charge);
      case CMP_NEXT_POINT_CHARGE:
        if (charge < next_point_charge)
          return interpolate(point_charge, point_voltage, slope, charge);

        point_charge = next_point_charge;
        points_iterator = next_points_iterator;

        if (charge == point_charge) {
          state = CMP_POINT_CHARGE;
          return point_voltage;
        }

        continue;
      }

      point_charge = next_point_charge;
      points_iterator = next_points_iterator;

      if (charge == point_charge) {
        point_voltage = points_iterator->cell_voltage;
        state = CMP_POINT_CHARGE;
        return point_voltage;
      }
    }
  }
}

FloatingPointType interpolate(const FloatingPointType x_0,
                              const FloatingPointType v_0,
                              const FloatingPointType slope,
                              const FloatingPointType x_q) {
  return v_0 + (x_q - x_0) * slope;
}
