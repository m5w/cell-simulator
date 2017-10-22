#ifndef READ_H
#define READ_H

#include <istream>

template <class T> inline T read(std::istream &is);

template <class T> T read(std::istream &is) {
  T is_x;
  is.read(reinterpret_cast<char *>(&is_x), sizeof(is_x));
  return is_x;
}

#endif
