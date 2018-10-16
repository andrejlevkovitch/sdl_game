// texture.hpp

#pragma once

#include <cstddef>
#include <cstdint>

namespace levi {
class texture {
public:
  texture();
  texture(uint32_t gl_tex, size_t width, size_t height);

public:
  uint32_t gl_tex;
  size_t width;
  size_t height;
};
}; // namespace levi
