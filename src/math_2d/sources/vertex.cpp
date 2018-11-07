// vertex.cpp

#include "vertex.hpp"

levi::vertex::vertex() : x{}, y{}, z{} {}

levi::vertex::vertex(float x_, float y_, float z_) : x{x_}, y{y_}, z{z_} {}

levi::vertex::vertex(vector2d v, float z_) : x{v.x}, y{v.y}, z{z_} {}

levi::vertex levi::vertex::operator+(const vertex &rhs) const {
  return vertex(x + rhs.x, y + rhs.y, z + rhs.z);
}

std::ostream &levi::operator<<(std::ostream &out, const vertex &vertex) {
  out << "x: " << vertex.x << " y: " << vertex.y << " z: " << vertex.z
      << std::endl;
  return out;
}
