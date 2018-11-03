// circle.hpp

#include "vector2d.hpp"

namespace levi {
class circle;

bool is_crossing(const circle &lhs, const circle &rhs);

class circle {
public:
  circle();
  circle(vector2d pos, float radius);

  vector2d pos;
  float radius;
};
}; // namespace levi
