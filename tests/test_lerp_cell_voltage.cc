#include <cstddef>

#define BOOST_TEST_MODULE test_lerp_cell_voltage
#include <boost/test/included/unit_test.hpp>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"
#include "lerp_cell_voltage.h"

typedef struct CheckLerpCellVoltageBufType {
  const FloatingPointType charge_discharged_from_cell;
  const FloatingPointType cell_voltage_source_voltage;
  const ErrorType the_error;
} CheckLerpCellVoltageBufType;

static inline void check_lerp_cell_voltage(
    const FloatingPointType charge_discharged_from_cell,
    const FloatingPointType cell_voltage_source_voltage,
    const ErrorType the_error,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer);

BOOST_AUTO_TEST_CASE(test_lerp_cell_voltage) {
  BOOST_CHECK(error == NONE);

  const CellDischargeCurvePoint points[] = {
      {0.0f, 1.0f}, {1.0f, 2.0f}, {2.0f, 4.0f}};
  constexpr std::size_t number_of_points =
      sizeof(points) / sizeof(CellDischargeCurvePoint);

  LerpCellVoltageLinearBufType linear_buf =
      lerp_cell_voltage_linear_buf_type(points, number_of_points);
  BOOST_CHECK(error == NONE);
  LerpCellVoltageBinaryBufType binary_buf =
      lerp_cell_voltage_binary_buf_type(points, number_of_points);
  BOOST_CHECK(error == NONE);

  const CheckLerpCellVoltageBufType bufs[] = {
      {-2.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW},
      {-1.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW},
      {+0.00f, +1.00f, NONE},
      {+0.00f, +1.00f, NONE},
      {+0.50f, +1.50f, NONE},
      {+0.75f, +1.75f, NONE},
      {+1.00f, +2.00f, NONE},
      {+1.00f, +2.00f, NONE},
      {+1.50f, +3.00f, NONE},
      {+1.75f, +3.50f, NONE},
      {+2.00f, +4.00f, NONE},
      {+2.00f, +4.00f, NONE},
      {+3.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE},
      {+4.00F, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE},
      {+4.00F, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE},
      {+3.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE},
      {+2.00f, +4.00f, NONE},
      {+2.00f, +4.00f, NONE},
      {+1.75f, +3.50f, NONE},
      {+1.50f, +3.00f, NONE},
      {+1.00f, +2.00f, NONE},
      {+1.00f, +2.00f, NONE},
      {+0.75f, +1.75f, NONE},
      {+0.50f, +1.50f, NONE},
      {+0.00f, +1.00f, NONE},
      {+0.00f, +1.00f, NONE},
      {-1.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW},
      {-2.00f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW}};
  const CheckLerpCellVoltageBufType *const bufs_end =
      bufs + sizeof(bufs) / sizeof(CheckLerpCellVoltageBufType);

  for (const CheckLerpCellVoltageBufType *buf_pointer = bufs;
       buf_pointer < bufs_end; ++buf_pointer)
    check_lerp_cell_voltage(buf_pointer->charge_discharged_from_cell,
                            buf_pointer->cell_voltage_source_voltage,
                            buf_pointer->the_error, &linear_buf, &binary_buf);
}

void check_lerp_cell_voltage(
    const FloatingPointType charge_discharged_from_cell,
    const FloatingPointType cell_voltage_source_voltage,
    const ErrorType the_error,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer) {
  BOOST_CHECK(lerp_cell_voltage_linear(linear_buf_pointer,
                                       charge_discharged_from_cell) ==
              cell_voltage_source_voltage);
  BOOST_CHECK(error == the_error);
  error = NONE;
  BOOST_CHECK(lerp_cell_voltage_binary(binary_buf_pointer,
                                       charge_discharged_from_cell) ==
              cell_voltage_source_voltage);
  BOOST_CHECK(error == the_error);
  error = NONE;
}
