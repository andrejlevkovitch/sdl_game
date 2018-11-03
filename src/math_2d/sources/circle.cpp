// circle.cpp

#include "circle.hpp"

levi::circle::circle() : pos{}, radius{} {};

levi::circle::circle(vector2d pos_, float radius_)
    : pos{pos_}, radius{radius_} {}

bool levi::is_crossing(const circle &lhs, const circle &rhs) {
  auto dist = get_distance_bitwin(lhs.pos, rhs.pos);
  if (dist < lhs.radius + rhs.radius) {
    return true;
  }
  return false;
}
