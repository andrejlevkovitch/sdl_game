// vector2d.cpp

#include "vector2d.hpp"
#include <cmath>

levi::vector2d::vector2d() : x{}, y{} {}

levi::vector2d::vector2d(float x_, float y_) : x{x_}, y{y_} {}

bool levi::vector2d::operator==(const vector2d &rhs) const {
  if (std::fabs(this->x - rhs.x) < 0.01 && std::fabs(this->y - rhs.y) < 0.01) {
    return true;
  }
  return false;
}

float levi::vector2d::get_length() const {
  float retval = std::sqrt(x * x + y * y);
  if (retval == NAN) {
    return 0;
  }
  return retval;
}

levi::vector2d levi::vector2d::operator+(const vector2d &rhs) const {
  return vector2d{this->x + rhs.x, this->y + rhs.y};
}

levi::vector2d &levi::vector2d::operator+=(const vector2d &rhs) {
  this->x += rhs.x;
  this->y += rhs.y;
  return *this;
}

levi::vector2d levi::vector2d::operator-(const vector2d &rhs) const {
  return vector2d{this->x - rhs.x, this->y - rhs.y};
}

levi::vector2d &levi::vector2d::operator-=(const vector2d &rhs) {
  this->x -= rhs.x;
  this->y -= rhs.y;
  return *this;
}

float levi::vector2d::operator*(const vector2d &rhs) const {
  return (this->x * rhs.x + this->y * rhs.y);
}

float levi::get_angle_bitwin(vector2d lhs, vector2d rhs, vector2d begin_koord) {
  lhs -= begin_koord;
  rhs -= begin_koord;
  auto lhs_normilize = lhs.get_norm();
  auto rhs_normalize = rhs.get_norm();
  return to_degrees(std::acos(lhs_normilize * rhs_normalize));
  ;
}

levi::vector2d levi::vector2d::operator*(float rhs) const {
  return vector2d{this->x * rhs, this->y * rhs};
}

levi::vector2d &levi::vector2d::operator*=(float rhs) {
  this->x *= rhs;
  this->y *= rhs;
  return *this;
}

levi::vector2d levi::operator*(float lhs, const levi::vector2d &rhs) {
  return vector2d{rhs.x * lhs, rhs.y * lhs};
}

levi::vector2d levi::vector2d::get_norm() const {
  auto length = get_length();
  if (!length) {
    return vector2d{0, 0};
  }
  return vector2d{this->x / length, this->y / length};
}

#include <iostream>

levi::vector2d levi::vector2d::get_vec_from(float distance, float angle) const {
  return vector2d{distance * std::cos(to_radians(angle)),
                  distance * std::sin(to_radians(angle))};
}

float levi::to_degrees(float radians) { return radians * 180.0f / 3.14f; }
float levi::to_radians(float degrees) { return degrees * 3.14f / 180.0f; }

std::ostream &levi::operator<<(std::ostream &out, const vector2d &vector) {
  out << "x: " << vector.x << " y: " << vector.y << std::endl;
  return out;
}
