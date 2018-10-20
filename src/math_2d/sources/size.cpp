// size.cpp

#include "size.hpp"

levi::size::size() : width{}, height{} {}

levi::size::size(int width_, int height_) : width{width_}, height{height_} {}

std::ostream &levi::operator<<(std::ostream &out, const size &size) {
  out << "width: " << size.width << " height: " << size.height << std::endl;
  return out;
}
