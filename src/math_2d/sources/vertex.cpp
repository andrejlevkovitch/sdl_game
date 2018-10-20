// vertex.cpp

#include "vertex.hpp"

levi::vertex::vertex() : x{}, y{}, z{} {}

levi::vertex::vertex(float x_, float y_, float z_) : x{x_}, y{y_}, z{z_} {}

std::ostream &levi::operator<<(std::ostream &out, const vertex &vertex) {
  out << "x: " << vertex.x << " y: " << vertex.y << " z: " << vertex.z
      << std::endl;
  return out;
}
