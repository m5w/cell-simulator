#ifndef MODULE_H
#define MODULE_H

#include <cstddef>

#include <array>

#include "cell_buf.h"

#include "floating_point_type.h"

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
      const FloatingPointType load_current, FloatingPointType &initial_work,
      FloatingPointType &voltage);
  FloatingPointType get_next_voltage(const FloatingPointType change_in_time);
  inline FloatingPointType get_load_work() const;
  inline FloatingPointType get_work() const;

private:
  static FloatingPointType
  get_sum(const std::array<FloatingPointType, number_of_cells> &cells_x);
  inline FloatingPointType
  get_voltage(const std::array<FloatingPointType, number_of_cells>
                  &cells_open_circuit_voltage);

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

FloatingPointType Module::get_load_work() const { return load_work; }

FloatingPointType Module::get_work() const {
  FloatingPointType work = 0.0;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    work += cells_buf[n].get_work();

  return work;
}

FloatingPointType
Module::get_voltage(const std::array<FloatingPointType, number_of_cells>
                        &cells_open_circuit_voltage) {
  // \sum_{n = 1}^\textrm{number of cells} v_n G_n
  //
  // A cell's current is
  //
  //   (v_n - v) G_n ,
  //
  // where v is the cell's open-circuit voltage,
  //       v is the module voltage, and
  //     G_n is the cell's mean internal conductance.
  //
  // The module voltage is the voltage of the node that connects all the cells'
  // internal resistors.  Suppose that v were 0 V; then the cell's current
  // would be
  //
  //   v_n G_n .
  //
  // Since the ground node connects all the cells' ideal voltage sources at
  // their negative terminals, for the module voltage to be 0 V would be for
  // the module to be connected to a short circuit.
  FloatingPointType sum_of_cells_short_circuit_current = 0.0;

  for (std::size_t n = 0; n < number_of_cells; ++n)
    sum_of_cells_short_circuit_current +=
        cells_open_circuit_voltage[n] * cells_mean_internal_conductance[n];

  return (sum_of_cells_short_circuit_current - load_current) /
         sum_of_cells_mean_internal_conductance;
}

#endif
