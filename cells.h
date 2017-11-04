#ifndef CELLS_H
#define CELLS_H

#include <cstddef>

#include <array>
#include <iosfwd>
#include <map>

#include "floating_point_type.h"

#include "cell.h"
#include "module.h"

class CellBuf;

class Cells {
private:
  template <std::size_t N> class CellBuf_array {
  public:
    CellBuf the_cell_buf;
    CellBuf_array<N - 1> the_cell_buf_array;
  };

public:
  Cells(std::istream &is);
  Module
  create_module(const std::array<std::size_t, number_of_cells> cells_identifer,
                const FloatingPointType load_current,
                FloatingPointType &voltage);

private:
  template <std::size_t N>
  CellBuf_array<N> create_cell_buf_array(
      const std::array<std::size_t, number_of_cells> &cells_identifer,
      std::array<FloatingPointType, number_of_cells> &cells_initial_work,
      std::array<FloatingPointType, number_of_cells>
          &cells_mean_internal_conductance,
      std::array<FloatingPointType, number_of_cells>
          &cells_open_circuit_voltage);
  CellBuf create_cell_buf(const std::size_t cell_identifier,
                          FloatingPointType &initial_work,
                          FloatingPointType &mean_internal_conductance,
                          FloatingPointType &open_circuit_voltage) const;

private:
  std::map<std::size_t, Cell> cells;
};

template <> class Cells::CellBuf_array<1> {
public:
  CellBuf the_cell_buf;
};

template <> class Cells::CellBuf_array<number_of_cells> {
public:
  operator std::array<CellBuf, number_of_cells> const();

private:
  template <std::size_t N, std::size_t... n> class GetCellsBuf {
  public:
    static std::array<CellBuf, number_of_cells>
    get_cells_buf(const CellBuf *const cells_buf);
  };

public:
  CellBuf the_cell_buf;
  CellBuf_array<number_of_cells - 1> the_cell_buf_array;
};

template <std::size_t... n>
class Cells::CellBuf_array<number_of_cells>::GetCellsBuf<1, n...> {
public:
  static std::array<CellBuf, number_of_cells>
  get_cells_buf(const CellBuf *const cells_buf);
};

template class Cells::CellBuf_array<number_of_cells>::GetCellsBuf<
    number_of_cells>;

template <>
Cells::CellBuf_array<1> Cells::create_cell_buf_array<1>(
    const std::array<std::size_t, number_of_cells> &cells_identifer,
    std::array<FloatingPointType, number_of_cells> &cells_initial_work,
    std::array<FloatingPointType, number_of_cells>
        &cells_mean_internal_conductance,
    std::array<FloatingPointType, number_of_cells>
        &cells_open_circuit_voltage);

template Cells::CellBuf_array<number_of_cells>
Cells::create_cell_buf_array<number_of_cells>(
    const std::array<std::size_t, number_of_cells> &cells_identifer,
    std::array<FloatingPointType, number_of_cells> &cells_initial_work,
    std::array<FloatingPointType, number_of_cells>
        &cells_mean_internal_conductance,
    std::array<FloatingPointType, number_of_cells>
        &cells_open_circuit_voltage);

#endif
