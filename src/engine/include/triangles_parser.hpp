// triangles_parser.hpp

#include <cstdint>
#include <string>
#include <vector>

#include "vertex.hpp"

#pragma once

namespace levi {
bool triangles_parser(const std::string &file_name,
                      std::vector<vertex> &vertices,
                      std::vector<uint32_t> &indexes);
};
