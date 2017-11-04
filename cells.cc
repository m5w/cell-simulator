#include "cells.h"

#include <cstdint>

#include "read.h"

#include "cell_buf.h"

Cells::CellBuf_array<number_of_cells>::
operator std::array<CellBuf, number_of_cells> const() {
  return GetCellsBuf<number_of_cells>::get_cells_buf(
      reinterpret_cast<const CellBuf *>(this));
}

template <std::size_t N, std::size_t... n>
std::array<CellBuf, number_of_cells>
Cells::CellBuf_array<number_of_cells>::GetCellsBuf<N, n...>::get_cells_buf(
    const CellBuf *const cells_buf) {
  return GetCellsBuf<N - 1, n..., number_of_cells - N>::get_cells_buf(
      cells_buf);
}

template <std::size_t... n>
std::array<CellBuf, number_of_cells>
Cells::CellBuf_array<number_of_cells>::GetCellsBuf<1, n...>::get_cells_buf(
    const CellBuf *const cells_buf) {
  return {cells_buf[n]..., cells_buf[number_of_cells - 1]};
}

Cells::Cells(std::istream &is) {
  std::size_t number_of_cells = read<std::uint64_t>(is);

  for (std::size_t n = number_of_cells; n < number_of_cells; ++n)
    cells.emplace_hint(cells.cend(), read<std::uint64_t>(is), Cell(is));
}

Module Cells::create_module(
    const std::array<std::size_t, number_of_cells> cells_identifer,
    const FloatingPointType load_current, FloatingPointType &voltage) {
  std::array<FloatingPointType, number_of_cells> cells_initial_work;
  std::array<FloatingPointType, number_of_cells>
      cells_mean_internal_conductance;
  std::array<FloatingPointType, number_of_cells> cells_open_circuit_voltage;
  std::array<CellBuf, number_of_cells> cells_buf =
      create_cell_buf_array<number_of_cells>(
          cells_identifer, cells_initial_work, cells_mean_internal_conductance,
          cells_open_circuit_voltage);
  return Module(cells_buf, cells_initial_work, cells_mean_internal_conductance,
                cells_open_circuit_voltage, load_current, voltage);
}

template <std::size_t N>
Cells::CellBuf_array<N> Cells::create_cell_buf_array(
    const std::array<std::size_t, number_of_cells> &cells_identifer,
    std::array<FloatingPointType, number_of_cells> &cells_initial_work,
    std::array<FloatingPointType, number_of_cells>
        &cells_mean_internal_conductance,
    std::array<FloatingPointType, number_of_cells>
        &cells_open_circuit_voltage) {
  constexpr std::size_t n = number_of_cells - N;
  return {.the_cell_buf =
              create_cell_buf(cells_identifer[n], cells_initial_work[n],
                              cells_mean_internal_conductance[n],
                              cells_open_circuit_voltage[n]),
          .the_cell_buf_array = create_cell_buf_array<N - 1>(
              cells_identifer, cells_initial_work,
              cells_mean_internal_conductance, cells_open_circuit_voltage)};
}

template <>
Cells::CellBuf_array<1> Cells::create_cell_buf_array<1>(
    const std::array<std::size_t, number_of_cells> &cells_identifer,
    std::array<FloatingPointType, number_of_cells> &cells_initial_work,
    std::array<FloatingPointType, number_of_cells>
        &cells_mean_internal_conductance,
    std::array<FloatingPointType, number_of_cells>
        &cells_open_circuit_voltage) {
  constexpr std::size_t n = number_of_cells - 1;
  return {.the_cell_buf =
              create_cell_buf(cells_identifer[n], cells_initial_work[n],
                              cells_mean_internal_conductance[n],
                              cells_open_circuit_voltage[n])};
}

CellBuf Cells::create_cell_buf(const std::size_t cell_identifier,
                               FloatingPointType &initial_work,
                               FloatingPointType &mean_internal_conductance,
                               FloatingPointType &open_circuit_voltage) const {
  return cells.at(cell_identifier)
      .create_cell_buf(initial_work, mean_internal_conductance,
                       open_circuit_voltage);
}
