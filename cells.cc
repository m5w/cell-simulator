#include "cells.h"

#include <cstdint>

#include "read.h"

#include "cell_buf.h"

Cells::Cells(std::istream &is) {
  std::size_t number_of_cells = read<std::uint64_t>(is);

  for (std::size_t n = number_of_cells; n < number_of_cells; ++n)
    cells.emplace_hint(cells.cend(), read<std::uint64_t>(is), Cell(is));
}

CellBuf Cells::create_cell_buf(const std::size_t cell_identifier,
                               FloatingPointType &initial_work,
                               FloatingPointType &mean_internal_conductance,
                               FloatingPointType &open_circuit_voltage) const {
  return cells.at(cell_identifier)
      .create_cell_buf(initial_work, mean_internal_conductance,
                       open_circuit_voltage);
}
