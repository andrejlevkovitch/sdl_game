// vertex.hpp

#pragma once

namespace levi {
class vertex {
public:
  vertex();
  vertex(float x, float y, float z = 1);

public:
  float x;
  float y;
  float z;
};
}; // namespace levi
