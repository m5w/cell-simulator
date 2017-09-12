#include <cstddef>

#define BOOST_TEST_MODULE test_interpolate_cell_voltage
#include <boost/test/included/unit_test.hpp>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"
#include "interpolate_cell_voltage.h"

BOOST_AUTO_TEST_CASE(test_interpolate_cell_voltage) {
  const CellDischargeCurvePoint points[] = {{0, 1}, {1, 2}, {2, 4}};
  constexpr std::size_t number_of_points =
      sizeof(points) / sizeof(CellDischargeCurvePoint);
  FloatingPointType voltage;

  BOOST_CHECK(error == NONE);

  voltage = interpolate_cell_voltage(points, number_of_points, -2);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  error = NONE;

  voltage = interpolate_cell_voltage(points, number_of_points, -1);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);
  error = NONE;

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = interpolate_cell_voltage(points, number_of_points, 0);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 1);
  }

  voltage = interpolate_cell_voltage(points, number_of_points, 0.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.5);

  voltage = interpolate_cell_voltage(points, number_of_points, 0.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 1.75);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = interpolate_cell_voltage(points, number_of_points, 1);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 2);
  }

  voltage = interpolate_cell_voltage(points, number_of_points, 1.5);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3);

  voltage = interpolate_cell_voltage(points, number_of_points, 1.75);
  BOOST_CHECK(error == NONE);
  BOOST_CHECK(voltage == 3.5);

  for (std::size_t x = 0; x < 2; ++x) {
    voltage = interpolate_cell_voltage(points, number_of_points, 2);
    BOOST_CHECK(error == NONE);
    BOOST_CHECK(voltage == 4);
  }

  voltage = interpolate_cell_voltage(points, number_of_points, 3);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  error = NONE;

  voltage = interpolate_cell_voltage(points, number_of_points, 4);
  BOOST_CHECK(error == EXTRAPOLATION_ABOVE);
  error = NONE;
}
