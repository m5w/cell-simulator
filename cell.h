#ifndef CELL_H
#define CELL_H

#include <iosfwd>

#include "floating_point_type.h"

#include "cell_points.h"

class CellBuf;

class Cell {
public:
  Cell(std::istream &is);
  CellBuf create_cell_buf(FloatingPointType &initial_work,
                          FloatingPointType &mean_internal_conductance,
                          FloatingPointType &open_circuit_voltage) const;

private:
  CellPoints the_cell_points;

public:
  const FloatingPointType mean_internal_conductance;
};

#endif
