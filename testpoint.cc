#include <cstddef>
#include <cmath>

#define BOOST_TEST_MODULE testpoint
#include <boost/test/included/unit_test.hpp>

#include "point.h"

BOOST_AUTO_TEST_CASE(testpoint_test_interpolate_voltage) {
  BatteryDischargeCurvePoint points[] = {
      {-96, 12},  {-94, 12}, {-72, 127}, {-50, 119}, {33, 119},
      {80, -104}, {85, -80}, {103, -57}, {105, 117}, {120, 126}};
  const std::size_t number_of_points =
      sizeof(points) / sizeof(BatteryDischargeCurvePoint);
  FloatingPointType voltage;

  error = NONE;
  interpolate_voltage(points, number_of_points, -100);
  BOOST_CHECK(error == EXTRAPOLATION_BELOW);

  error = NONE;
  voltage = interpolate_voltage(points, number_of_points, -88);
  BOOST_CHECK(error == NONE);
}
