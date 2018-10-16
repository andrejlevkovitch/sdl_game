// rect.cpp

#include "rect.hpp"

#include "vertex.hpp"

levi::rect::rect() : x{}, y{}, width{}, height{} {}

levi::rect::rect(int x_, int y_, int width_, int height_)
    : x{x_}, y{y_}, width{width_}, height{height_} {}

std::array<levi::vertex, 4> levi::rect::get_vertices() const {
  return std::array<vertex, 4>{vertex(x, y), vertex(x, y + height),
                               vertex(x + width, y + height),
                               vertex(x + width, y)};
}

levi::vertex levi::rect::get_center() const {
  return vertex(x + width / 2.0, y + height / 2.0);
}
