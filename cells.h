#ifndef CELLS_H
#define CELLS_H

#include <cstddef>

#include <iosfwd>
#include <map>

#include "floating_point_type.h"

#include "cell.h"

class CellBuf;

class Cells {
public:
  Cells(std::istream &is);
  CellBuf create_cell_buf(const std::size_t cell_identifier,
                          FloatingPointType &initial_work,
                          FloatingPointType &mean_internal_conductance,
                          FloatingPointType &open_circuit_voltage) const;

private:
  std::map<std::size_t, Cell> cells;
};

#endif
