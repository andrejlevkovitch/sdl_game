// size.hpp

#pragma once

#include <ostream>

namespace levi {
class size {
public:
  size();
  size(int width, int height);

public:
  int width;
  int height;
};

std::ostream &operator<<(std::ostream &out, const size &size);
}; // namespace levi
