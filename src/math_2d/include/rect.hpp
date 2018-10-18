// rect.hpp

#pragma once

#include <array>

namespace levi {
class vertex;

class rect {
public:
  rect();
  rect(int x, int y, int width, int height);
  /**\return return array of vertices of rectangle, begin from left-top corner
   * oposit-clock-wise. [0,0] in top-left corner*/
  std::array<vertex, 4> get_vertices() const;
  vertex get_center() const;
  friend bool is_crossing(const rect &lhs, const rect &rhs);

  int x;
  int y;
  int width;
  int height;
};
bool is_crossing(const rect &lhs, const rect &rhs);
} // namespace levi
