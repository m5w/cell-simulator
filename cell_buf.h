#ifndef CELL_BUF_H
#define CELL_BUF_H

#include "floating_point_type.h"

#include "cell_points_buf.h"

class Cell;

class CellBuf {
public:
  CellBuf(const Cell *const the_cell_pointer,
          CellPointsBuf the_cell_points_buf,
          FloatingPointType &mean_internal_conductance,
          FloatingPointType &open_circuit_voltage);
  FloatingPointType
  get_next_open_circuit_voltage(const FloatingPointType change_in_time,
                                const FloatingPointType module_voltage);
  FloatingPointType get_work() const;

private:
  inline FloatingPointType
  get_open_circuit_voltage(const FloatingPointType charge_supplied);

private:
  CellPointsBuf the_cell_points_buf;
  FloatingPointType mean_internal_conductance;
  FloatingPointType charge_supplied;
  FloatingPointType open_circuit_voltage;
};

FloatingPointType
CellBuf::get_open_circuit_voltage(const FloatingPointType charge_supplied) {
  return the_cell_points_buf.lerp_open_circuit_voltage(charge_supplied);
}

#endif
