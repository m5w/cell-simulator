#include "cell_points.h"

#include <exception>

#include <boost/config.hpp>

#include "cell_points_buf.h"
#include "read.h"

#define READ                                                                  \
  const FloatingPointType the_charge_supplied = read<double>(is);             \
  const FloatingPointType the_open_circuit_voltage = read<double>(is);        \
  const FloatingPointType the_work = read<double>(is);                        \
  points_work.push_back(the_work)

#define READ_CREATE_POINT                                                     \
  {                                                                           \
    .x = the_charge_supplied, .y = the_open_circuit_voltage,                  \
    .z_pointer = &points_work.back()                                          \
  }

CellPoints::CellPoints(std::istream &is) {
  const std::size_t number_of_points = read<std::uint64_t>(is);

  if (BOOST_UNLIKELY(0 >= number_of_points))
    throw std::exception();

  points.reserve(number_of_points);
  points_work.reserve(number_of_points);

  READ;
  initial_work = the_work;
  points.push_back(READ_CREATE_POINT);

  for (std::size_t n = 1; n < number_of_points; ++n)
    points.push_back(read_point(is));
}

CellPointsBuf
CellPoints::create_cell_points_buf(FloatingPointType &initial_work) const {
  return CellPointsBuf(this, initial_work);
}

Point CellPoints::read_point(std::istream &is) {
  READ;
  return READ_CREATE_POINT;
}

#undef READ
#undef READ_CREATE_POINT
