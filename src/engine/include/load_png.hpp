// load_png.hpp

#pragma once

#include <string>
#include <vector>

namespace levi {
struct image {
  std::vector<unsigned char> data;
  size_t width;
  size_t height;
};

/**\exception if couldn't open or read file*/
image load_png_as_rgb(const std::string &file);
}; // namespace levi
