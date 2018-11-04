// rect.hpp

#pragma once

#include <array>
#include <ostream>

namespace levi {
class vertex;
class vector2d;

class rect {
public:
  rect();
  rect(int x, int y, int width, int height);
  /**\return return array of vertices of rectangle, begin from left-top corner
   * oposit-clock-wise. [0,0] in top-left corner.
   * \param z_value z koord for vertices, by default = 0*/
  std::array<vertex, 4> get_vertices(float z_value = 0) const;
  vertex get_center() const;
  friend bool is_crossing(const rect &lhs, const rect &rhs);
  /**\return true, if pos is in rectangle, or is on left or top side*/
  bool is_intake_pos(const vector2d &pos);

  int x;
  int y;
  int width;
  int height;
};

bool is_crossing(const rect &lhs, const rect &rhs);

std::ostream &operator<<(std::ostream &out, const rect &rect);
} // namespace levi
