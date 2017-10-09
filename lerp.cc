#include "lerp.h"

bool operator==(const LerpLinearBuf &lhs, const LerpLinearBuf &rhs) {
  return lhs.state == rhs.state && lhs.points_end == rhs.points_end &&
         lhs.points_iterator == rhs.points_iterator &&
         lhs.point_x == rhs.point_x &&
         lhs.next_points_iterator == rhs.next_points_iterator &&
         lhs.next_point_x == rhs.next_point_x && lhs.point_y == rhs.point_y &&
         lhs.m == rhs.m && lhs.points_begin == rhs.points_begin;
}

bool operator==(const LerpBinaryBuf &lhs, const LerpBinaryBuf &rhs) {
  return lhs.points_front_x == rhs.points_front_x &&
         lhs.points_back_x == rhs.points_back_x && lhs.state == rhs.state &&
         lhs.points_front_pointer == rhs.points_front_pointer &&
         lhs.points_back_pointer == rhs.points_back_pointer &&
         lhs.points_iterator == rhs.points_iterator &&
         lhs.point_x == rhs.point_x && lhs.point_y == rhs.point_y &&
         lhs.m == rhs.m;
}
