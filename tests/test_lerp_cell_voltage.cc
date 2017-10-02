#include <cstddef>

#define BOOST_TEST_MODULE test_lerp_cell_voltage
#include <boost/test/included/unit_test.hpp>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"
#include "lerp_cell_voltage.h"

static inline void check_lerp_cell_voltage(
    const FloatingPointType charge, const ErrorType the_error,
    const FloatingPointType the_voltage,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer);

BOOST_AUTO_TEST_CASE(test_lerp_cell_voltage) {
  BOOST_CHECK(error == NONE);

  const CellDischargeCurvePoint points[] = {
      {0.0f, 1.0f}, {1.0f, 2.0f}, {2.0f, 4.0f}};
  constexpr std::size_t number_of_points =
      sizeof(points) / sizeof(CellDischargeCurvePoint);
  FloatingPointType linear_voltage;
  FloatingPointType binary_voltage;

  LerpCellVoltageLinearBufType linear_buf =
      lerp_cell_voltage_linear_buf_type(points, number_of_points);
  BOOST_CHECK(error == NONE);
  LerpCellVoltageBinaryBufType binary_buf =
      lerp_cell_voltage_binary_buf_type(points, number_of_points);
  BOOST_CHECK(error == NONE);

#define CHECK_LERP_CELL_VOLTAGE(the_error, charge, the_voltage)               \
  check_lerp_cell_voltage((charge), (the_error), (the_voltage), &linear_buf,  \
                          &binary_buf)

  CHECK_LERP_CELL_VOLTAGE(EXTRAPOLATION_BELOW, -2.00f,
                          ERROR_FLOATING_POINT_TYPE);
  CHECK_LERP_CELL_VOLTAGE(EXTRAPOLATION_BELOW, -1.00f,
                          ERROR_FLOATING_POINT_TYPE);

  for (std::size_t x = 0; x < 2; ++x)
    CHECK_LERP_CELL_VOLTAGE(NONE, +0.00f, +1.00f);

  CHECK_LERP_CELL_VOLTAGE(NONE, 0.50f, 1.50f);
  CHECK_LERP_CELL_VOLTAGE(NONE, 0.75f, 1.75f);

  for (std::size_t x = 0; x < 2; ++x)
    CHECK_LERP_CELL_VOLTAGE(NONE, 1.00f, 2.00f);

  CHECK_LERP_CELL_VOLTAGE(NONE, 1.50f, 3.00f);
  CHECK_LERP_CELL_VOLTAGE(NONE, 1.75f, 3.50f);

  for (std::size_t x = 0; x < 2; ++x)
    CHECK_LERP_CELL_VOLTAGE(NONE, 2.00f, 4.00f);

  CHECK_LERP_CELL_VOLTAGE(EXTRAPOLATION_ABOVE, 3.00f,
                          ERROR_FLOATING_POINT_TYPE);
  CHECK_LERP_CELL_VOLTAGE(EXTRAPOLATION_ABOVE, 4.00f,
                          ERROR_FLOATING_POINT_TYPE);
}

void check_lerp_cell_voltage(
    const FloatingPointType charge, const ErrorType the_error,
    const FloatingPointType the_voltage,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer) {
  BOOST_CHECK(lerp_cell_voltage_linear(linear_buf_pointer, charge) ==
              the_voltage);
  BOOST_CHECK(error == the_error);
  error = NONE;
  BOOST_CHECK(lerp_cell_voltage_binary(binary_buf_pointer, charge) ==
              the_voltage);
  BOOST_CHECK(error == the_error);
  error = NONE;
}
