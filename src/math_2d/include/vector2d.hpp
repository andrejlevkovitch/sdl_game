// vector2d.hpp

#pragma once

namespace levi {
class vector2d final {
public:
  vector2d();
  vector2d(float x, float y);
  void set_x(float x);
  float get_x() const;
  void set_y(float y);
  float get_y() const;
  /**\warning return true only if x_ and y_ completely identical
   * \todo close fraction for this method*/
  bool operator==(const vector2d &rhs) const;
  /**\brief compute length every time, when you call this method*/
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
  /**\return normilise copy of this vector2d*/
  vector2d get_norm() const;
  /**\return new vector which is on a distance 'distance' (relatively this) and
   * at an angle 'angle' (relatively axis y)*/
  vector2d get_vec_from(float distance, float angle) const;

private:
  float x_;
  float y_;
};

float get_angle_bitwin(vector2d lhs, vector2d rhs,
                       vector2d begin_koord = vector2d{0, 0});
}; // namespace levi
