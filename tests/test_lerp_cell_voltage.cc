#include <cstddef>
#include <string>
#include <vector>

#define BOOST_TEST_MODULE test_liblerp
#include <boost/test/included/unit_test.hpp>

#include "floating_point_type.h"
#include "lerp.h"
#include "point.h"

typedef struct TestLerpPointErrorBuf {
  const FloatingPointType x;
  const FloatingPointType y;
  const LerpError lerp_error;
} TestLerpPointErrorBuf;

static void test_lerp_point_error(const TestLerpPointErrorBuf &buf,
                                  LerpLinearBuf &linear_buf,
                                  LerpBinaryBuf &binary_buf);

BOOST_AUTO_TEST_CASE(test_lerp) {
  BOOST_REQUIRE(lerp_error == NONE);

  const std::vector<Point> points = {{1.0f, 2.0f}, {2.0f, 4.0f}, {3.0f, 8.0f}};

  LerpLinearBuf linear_buf = lerp_linear_buf(points.data(), points.size());
  BOOST_REQUIRE(lerp_error == NONE);
  LerpBinaryBuf binary_buf = lerp_binary_buf(points.data(), points.size());
  BOOST_REQUIRE(lerp_error == NONE);

  const std::vector<TestLerpPointErrorBuf> test_lerp_bufs = {
      {0.5f, DEFAULT_FLOATING_POINT_TYPE, EXTRAPOLATION_BELOW},
      {1.0f, 2.0f, NONE},
      {1.5f, 3.0f, NONE},
      {2.0f, 4.0f, NONE},
      {2.5f, 6.0f, NONE},
      {3.0f, 8.0f, NONE},
      {3.5f, DEFAULT_FLOATING_POINT_TYPE, EXTRAPOLATION_ABOVE}};

  std::vector<std::size_t> test_lerp_bufs_indices(test_lerp_bufs.size(), 0);
  std::vector<LerpLinearBuf> linear_bufs(test_lerp_bufs.size());
  linear_bufs.at(0) = linear_buf;
  std::vector<LerpBinaryBuf> binary_bufs(test_lerp_bufs.size());
  binary_bufs.at(0) = binary_buf;
  std::size_t test_lerp_bufs_indices_index = 0;

  for (;;) {
    if (test_lerp_bufs_indices.at(test_lerp_bufs_indices_index) >=
        test_lerp_bufs.size()) {
      if (test_lerp_bufs_indices_index == 0)
        break;

      --test_lerp_bufs_indices_index;
      continue;
    }

    std::cout << std::string(test_lerp_bufs_indices_index, ' ')
              << test_lerp_bufs_indices.at(test_lerp_bufs_indices_index)
              << '\n';
    LerpLinearBuf the_linear_buf =
        linear_bufs.at(test_lerp_bufs_indices_index);
    LerpBinaryBuf the_binary_buf =
        binary_bufs.at(test_lerp_bufs_indices_index);
    test_lerp_point_error(test_lerp_bufs.at(test_lerp_bufs_indices.at(
                              test_lerp_bufs_indices_index)),
                          the_linear_buf, the_binary_buf);
    ++test_lerp_bufs_indices.at(test_lerp_bufs_indices_index);
    ++test_lerp_bufs_indices_index;

    if (test_lerp_bufs_indices_index >= test_lerp_bufs.size()) {
      --test_lerp_bufs_indices_index;
      continue;
    }

    test_lerp_bufs_indices.at(test_lerp_bufs_indices_index) = 0;
    linear_bufs.at(test_lerp_bufs_indices_index) = the_linear_buf;
    binary_bufs.at(test_lerp_bufs_indices_index) = the_binary_buf;
  }
}

void test_lerp_point_error(const TestLerpPointErrorBuf &buf,
                           LerpLinearBuf &linear_buf,
                           LerpBinaryBuf &binary_buf) {
  if (buf.lerp_error != NONE) {
    const LerpLinearBuf the_linear_buf = linear_buf;
    BOOST_REQUIRE_EQUAL(lerp_linear(&linear_buf, buf.x), buf.y);
    BOOST_REQUIRE_EQUAL(lerp_error, buf.lerp_error);
    lerp_error = NONE;
    BOOST_REQUIRE(linear_buf == the_linear_buf);
    const LerpBinaryBuf the_binary_buf = binary_buf;
    BOOST_REQUIRE_EQUAL(lerp_binary(&binary_buf, buf.x), buf.y);
    BOOST_REQUIRE_EQUAL(lerp_error, buf.lerp_error);
    lerp_error = NONE;
    BOOST_REQUIRE(binary_buf == the_binary_buf);
  } else {
    BOOST_REQUIRE_EQUAL(lerp_linear(&linear_buf, buf.x), buf.y);
    BOOST_REQUIRE_EQUAL(lerp_error, NONE);
    BOOST_REQUIRE_EQUAL(lerp_binary(&binary_buf, buf.x), buf.y);
    BOOST_REQUIRE_EQUAL(lerp_error, NONE);
  }
}
