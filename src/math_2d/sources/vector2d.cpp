// vector2d.cpp

#include "vector2d.hpp"

#include <cmath>

namespace levi {
float to_degrees(float radians) { return radians * 180.0f / 3.14f; }
float to_radians(float degrees) { return degrees * 3.14f / 180.0f; }
}; // namespace levi

levi::vector2d::vector2d() : x_{}, y_{} {}

levi::vector2d::vector2d(float x, float y) : x_{x}, y_{y} {}

void levi::vector2d::set_x(float x) { x_ = x; }

float levi::vector2d::get_x() const { return x_; }

void levi::vector2d::set_y(float y) { y_ = y; }

float levi::vector2d::get_y() const { return y_; }

bool levi::vector2d::operator==(const vector2d &rhs) const {
  return ((this->x_ == rhs.x_) && (this->y_ == rhs.y_)) ? true : false;
}

float levi::vector2d::get_length() const {
  return std::sqrt(x_ * x_ + y_ * y_);
}

levi::vector2d levi::vector2d::operator+(const vector2d &rhs) const {
  return vector2d{this->x_ + rhs.x_, this->y_ + rhs.y_};
}

levi::vector2d &levi::vector2d::operator+=(const vector2d &rhs) {
  this->x_ += rhs.x_;
  this->y_ += rhs.y_;
  return *this;
}

levi::vector2d levi::vector2d::operator-(const vector2d &rhs) const {
  return vector2d{this->x_ - rhs.x_, this->y_ - rhs.y_};
}

levi::vector2d &levi::vector2d::operator-=(const vector2d &rhs) {
  this->x_ -= rhs.x_;
  this->y_ -= rhs.y_;
  return *this;
}

float levi::vector2d::operator*(const vector2d &rhs) const {
  return (this->x_ * rhs.x_ + this->y_ * rhs.y_);
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
  return vector2d{this->x_ * rhs, this->y_ * rhs};
}

levi::vector2d &levi::vector2d::operator*=(float rhs) {
  this->x_ *= rhs;
  this->y_ *= rhs;
  return *this;
}

levi::vector2d levi::operator*(float lhs, const levi::vector2d &rhs) {
  return vector2d{rhs.x_ * lhs, rhs.y_ * lhs};
}

levi::vector2d levi::vector2d::get_norm() const {
  auto length = get_length();
  return vector2d{this->x_ / length, this->y_ / length};
}

#include <iostream>

levi::vector2d levi::vector2d::get_vec_from(float distance, float angle) const {
  return vector2d{distance * std::cos(to_radians(angle)),
                  distance * std::sin(to_radians(angle))};
}
