#include <cstddef>

#define BOOST_TEST_MODULE test_lerp_cell_voltage
#include <boost/test/included/unit_test.hpp>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"
#include "lerp_cell_voltage.h"

BOOST_AUTO_TEST_CASE(test_lerp_cell_voltage) {
  BOOST_CHECK(error == NONE);

  const CellDischargeCurvePoint points[] = {{0, 1}, {1, 2}, {2, 4}};
  constexpr std::size_t number_of_points =
      sizeof(points) / sizeof(CellDischargeCurvePoint);
  FloatingPointType voltage;

  LerpCellVoltageLinearBufType buf =
      lerp_cell_voltage_linear_buf_type(points, number_of_points);
  BOOST_CHECK(error == NONE);

  voltage = lerp_cell_voltage(&buf, -2);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  BOOST_CHECK(voltage == ERROR_FLOATING_POINT_TYPE);
  error = NONE;

  voltage = lerp_cell_voltage(&buf, -1);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  BOOST_CHECK(voltage == ERROR_FLOATING_POINT_TYPE);
  error = NONE;

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&buf, 0);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 1);
  }

  voltage = lerp_cell_voltage(&buf, 0.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.5);

  voltage = lerp_cell_voltage(&buf, 0.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.75);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&buf, 1);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 2);
  }

  voltage = lerp_cell_voltage(&buf, 1.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3);

  voltage = lerp_cell_voltage(&buf, 1.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3.5);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&buf, 2);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 4);
  }

  voltage = lerp_cell_voltage(&buf, 3);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  BOOST_CHECK(voltage == ERROR_FLOATING_POINT_TYPE);
  error = NONE;

  voltage = lerp_cell_voltage(&buf, 4);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  BOOST_CHECK(voltage == ERROR_FLOATING_POINT_TYPE);
  error = NONE;
}
