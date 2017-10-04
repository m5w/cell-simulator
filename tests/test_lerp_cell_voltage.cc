#include <cstddef>
#include <string>

#define BOOST_TEST_MODULE test_lerp_cell_voltage_source_voltage
#include <boost/test/included/unit_test.hpp>

#include "cell_discharge_curve_point.h"
#include "error.h"
#include "floating_point_type.h"
#include "lerp_cell_voltage.h"

static bool operator==(const CellDischargeCurvePoint &a,
                       const CellDischargeCurvePoint &b);
static bool operator==(const LerpCellVoltageLinearBufType &a,
                       const LerpCellVoltageLinearBufType &b);
static bool operator==(const LerpCellVoltageBinaryBufType &a,
                       const LerpCellVoltageBinaryBufType &b);

typedef struct CheckLerpCellVoltageBufType {
  const FloatingPointType charge_discharged_from_cell;
  const FloatingPointType cell_voltage_source_voltage;
  const ErrorType the_error;
} CheckLerpCellVoltageBufType;

static inline void test_lerp_cell_voltage_source_voltage_point_error(
    const FloatingPointType charge_discharged_from_cell,
    const FloatingPointType cell_voltage_source_voltage,
    const ErrorType the_error,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer);

BOOST_AUTO_TEST_CASE(test_lerp_cell_voltage_source_voltage) {
  BOOST_REQUIRE(error == NONE);

  const CellDischargeCurvePoint points[] = {
      {1.0f, 2.0f}, {2.0f, 4.0f}, {3.0f, 8.0f}};
  constexpr std::size_t number_of_points =
      sizeof(points) / sizeof(CellDischargeCurvePoint);

  LerpCellVoltageLinearBufType linear_buf =
      lerp_cell_voltage_linear_buf_type(points, number_of_points);
  BOOST_REQUIRE(error == NONE);
  LerpCellVoltageBinaryBufType binary_buf =
      lerp_cell_voltage_binary_buf_type(points, number_of_points);
  BOOST_REQUIRE(error == NONE);

  const CheckLerpCellVoltageBufType bufs[] = {
      /* 0 */ {0.5f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW},
      /* 1 */ {1.0f, 2.0f, NONE},
      /* 2 */ {1.5f, 3.0f, NONE},
      /* 3 */ {2.0f, 4.0f, NONE},
      /* 4 */ {2.5f, 6.0f, NONE},
      /* 5 */ {3.0f, 8.0f, NONE},
      /* 6 */ {3.5f, ERROR_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE}};
  const size_t number_of_bufs =
      sizeof(bufs) / sizeof(CheckLerpCellVoltageBufType);
  size_t bufs_indices[number_of_bufs] = {0};
  LerpCellVoltageLinearBufType linear_bufs[number_of_bufs];
  linear_bufs[0] = linear_buf;
  LerpCellVoltageBinaryBufType binary_bufs[number_of_bufs];
  binary_bufs[0] = binary_buf;

  size_t bufs_indices_index = 0;

  for (;;) {
    std::cerr << std::string(bufs_indices_index, ' ')
              << bufs_indices[bufs_indices_index] << '\n';
    LerpCellVoltageLinearBufType the_linear_buf =
        linear_bufs[bufs_indices_index];
    LerpCellVoltageBinaryBufType the_binary_buf =
        binary_bufs[bufs_indices_index];
    test_lerp_cell_voltage_source_voltage_point_error(
        bufs[bufs_indices[bufs_indices_index]].charge_discharged_from_cell,
        bufs[bufs_indices[bufs_indices_index]].cell_voltage_source_voltage,
        bufs[bufs_indices[bufs_indices_index]].the_error, &the_linear_buf,
        &the_binary_buf);
    ++bufs_indices[bufs_indices_index];

    if (bufs_indices[bufs_indices_index] >= number_of_bufs) {
      if (bufs_indices_index == 0)
        break;

      --bufs_indices_index;
      continue;
    }

    ++bufs_indices_index;

    if (bufs_indices_index >= number_of_bufs) {
      --bufs_indices_index;
      continue;
    }

    bufs_indices[bufs_indices_index] = 0;
    linear_bufs[bufs_indices_index] = the_linear_buf;
    binary_bufs[bufs_indices_index] = the_binary_buf;
  }
}

bool operator==(const CellDischargeCurvePoint &a,
                const CellDischargeCurvePoint &b) {
  return a.charge_discharged_from_cell == b.charge_discharged_from_cell &&
         a.cell_voltage == b.cell_voltage;
}

bool operator==(const LerpCellVoltageLinearBufType &a,
                const LerpCellVoltageLinearBufType &b) {
  return a.state == b.state && a.points_begin == b.points_begin &&
         a.points_end == b.points_end &&
         a.points_iterator == b.points_iterator &&
         a.point_charge == b.point_charge &&
         a.next_points_iterator == b.next_points_iterator &&
         a.next_point_charge == b.next_point_charge &&
         a.point_voltage == b.point_voltage && a.m == b.m;
}

bool operator==(const LerpCellVoltageBinaryBufType &a,
                const LerpCellVoltageBinaryBufType &b) {
  return a.points_front_charge == b.points_front_charge &&
         a.points_back_charge == b.points_back_charge && a.state == b.state &&
         a.points_front_pointer == b.points_front_pointer &&
         a.points_back_pointer == b.points_back_pointer &&
         a.points_iterator == b.points_iterator &&
         a.point_charge == b.point_charge &&
         a.point_voltage == b.point_voltage && a.m == b.m;
}

void test_lerp_cell_voltage_source_voltage_point_error(
    const FloatingPointType charge_discharged_from_cell,
    const FloatingPointType cell_voltage_source_voltage,
    const ErrorType the_error,
    LerpCellVoltageLinearBufType *const linear_buf_pointer,
    LerpCellVoltageBinaryBufType *const binary_buf_pointer) {
  if (the_error != NONE) {
    const LerpCellVoltageLinearBufType linear_buf = *linear_buf_pointer;
    BOOST_REQUIRE_EQUAL(lerp_cell_voltage_linear(linear_buf_pointer,
                                                 charge_discharged_from_cell),
                        cell_voltage_source_voltage);
    BOOST_REQUIRE_EQUAL(error, the_error);
    error = NONE;
    BOOST_REQUIRE(*linear_buf_pointer == linear_buf);
    const LerpCellVoltageBinaryBufType binary_buf = *binary_buf_pointer;
    BOOST_REQUIRE_EQUAL(lerp_cell_voltage_binary(binary_buf_pointer,
                                                 charge_discharged_from_cell),
                        cell_voltage_source_voltage);
    BOOST_REQUIRE_EQUAL(error, the_error);
    error = NONE;
    BOOST_REQUIRE(*binary_buf_pointer == binary_buf);
  } else {
    BOOST_REQUIRE_EQUAL(lerp_cell_voltage_linear(linear_buf_pointer,
                                                 charge_discharged_from_cell),
                        cell_voltage_source_voltage);
    BOOST_REQUIRE_EQUAL(error, NONE);
    BOOST_REQUIRE_EQUAL(lerp_cell_voltage_binary(binary_buf_pointer,
                                                 charge_discharged_from_cell),
                        cell_voltage_source_voltage);
    BOOST_REQUIRE_EQUAL(error, NONE);
  }
}
