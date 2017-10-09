#include "point.h"

bool operator==(const Point &lhs, const Point &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
