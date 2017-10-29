#include "cell.h"

#include "cell_buf.h"
#include "read.h"

Cell::Cell(std::istream &is)
    : the_cell_points(is), mean_internal_conductance(read<double>(is)) {}

CellBuf Cell::create_cell_buf(FloatingPointType &initial_work,
                              FloatingPointType &mean_internal_conductance,
                              FloatingPointType &open_circuit_voltage) const {
  return CellBuf(this, the_cell_points.create_cell_points_buf(initial_work),
                 mean_internal_conductance, open_circuit_voltage);
}
