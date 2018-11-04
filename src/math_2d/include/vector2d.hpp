// vector2d.hpp

#pragma once

#include <ostream>

namespace levi {
class vector2d final {
public:
  vector2d();
  vector2d(float x, float y);
  /**\return true if values close then 0.01, else - false*/
  bool operator==(const vector2d &rhs) const;
  /**\brief compute length every time, when you call this method
   * \return length of this vector. If x and y equal 0, then, garanted, this
   * method return 0*/
  float get_length() const;
  vector2d operator+(const vector2d &rhs) const;
  vector2d &operator+=(const vector2d &rhs);
  vector2d operator-(const vector2d &rhs) const;
  vector2d &operator-=(const vector2d &rhs);
  /**\return dot (scalar) product*/
  float operator*(const vector2d &rhs) const;
  /**\return angle bitwin vectors in degrees.
   * \param begin_koord angle bitwin vectors calculate relatively the vector, by
   * default {0, 0}*/
  friend float get_angle_bitwin(vector2d lhs, vector2d rhs,
                                vector2d begin_koord);
  vector2d operator*(float rhs) const;
  vector2d &operator*=(float rhs);
  friend vector2d operator*(float lhs, const vector2d &rhs);
  /**\return normilise copy of this vector2d. If x and y equal 0, then,
   * garanted, this method return vector2d{0,0}*/
  vector2d get_norm() const;
  /**\return new vector which is on a distance 'distance' (relatively this) and
   * at an angle 'angle' (relatively axis y)*/
  vector2d get_vec_from(float distance, float angle) const;

public:
  float x;
  float y;
};

float get_angle_bitwin(vector2d lhs, vector2d rhs,
                       vector2d begin_koord = vector2d{0, 0});
float get_distance_bitwin(vector2d lhs, vector2d rhs);

float to_degrees(float radians);
float to_radians(float degrees);
vector2d operator*(float lhs, const vector2d &rhs);

std::ostream &operator<<(std::ostream &out, const vector2d &vector);
}; // namespace levi
