// object_types.hpp

#pragma once

#include "abstract_object.hpp"
#include <string>
#include <vector>

namespace bombino {
enum object_type {
  error_value,
  user_type = levi::object_type::user_type,
  gamer1,
  gamer2,
  void_block,
  soft_block,
  solid_block,
  bomb,
  power
};

enum powers { energy, bombs, speed, size };

const std::vector<std::string> powers_names{"energy", "bombs", "speed"};
}; // namespace bombino
