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

  LerpCellVoltageType lerp_cell_voltage_state =
      lerp_cell_voltage_state_type(points, number_of_points);
  BOOST_CHECK(error == NONE);

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, -2);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  error = NONE;

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, -1);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  error = NONE;

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 0);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 1);
  }

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 0.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.5);

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 0.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.75);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 1);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 2);
  }

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 1.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3);

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 1.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3.5);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 2);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 4);
  }

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 3);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  error = NONE;

  voltage = lerp_cell_voltage(&lerp_cell_voltage_state, 4);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  error = NONE;
}
