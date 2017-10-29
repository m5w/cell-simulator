#ifndef CELL_POINTS_H
#define CELL_POINTS_H

#include <cstddef>
#include <cstdint>

#include <exception>
#include <istream>
#include <vector>

#include <boost/config.hpp>

#include "floating_point_type.h"
#include "point.h"

#include "read.h"

class CellPoints {
public:
  CellPoints(std::istream &is);
  inline FloatingPointType get_initial_work() const;
  inline const Point *get_points() const;
  inline std::size_t get_number_of_points() const;

private:
  Point read_point(std::istream &is);

private:
  std::vector<Point> points;
  std::vector<FloatingPointType> points_work;
  FloatingPointType initial_work;
};

FloatingPointType CellPoints::get_initial_work() const { return initial_work; }

const Point *CellPoints::get_points() const { return points.data(); }

std::size_t CellPoints::get_number_of_points() const { return points.size(); }

#endif
