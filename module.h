#ifndef MODULE_H
#define MODULE_H

#include <cstddef>

#include <array>

#include "floating_point_type.h"

class CellBuf;

constexpr std::size_t number_of_cells = 12;

class Module {
public:
  Module(
      const std::array<CellBuf, number_of_cells> cells_buf,
      const std::array<FloatingPointType, number_of_cells> cells_initial_work,
      const std::array<FloatingPointType, number_of_cells>
          cells_mean_internal_conductance,
      const std::array<FloatingPointType, number_of_cells>
          cells_open_circuit_voltage,
      const FloatingPointType load_current, FloatingPointType &voltage);
  FloatingPointType get_next_voltage(const FloatingPointType change_in_time);

private:
  static FloatingPointType get_sum_of_cells_mean_internal_conductance(
      const std::array<FloatingPointType, number_of_cells>
          &cells_mean_internal_conductance);
  FloatingPointType get_voltage();

private:
  std::array<CellBuf, number_of_cells> cells_buf;
  const std::array<FloatingPointType, number_of_cells> cells_initial_work;
  const std::array<FloatingPointType, number_of_cells>
      cells_mean_internal_conductance;
  std::array<FloatingPointType, number_of_cells> cells_open_circuit_voltage;
  const FloatingPointType load_current;
  const FloatingPointType sum_of_cells_mean_internal_conductance;
  FloatingPointType voltage;
  FloatingPointType load_work;
};

#endif
