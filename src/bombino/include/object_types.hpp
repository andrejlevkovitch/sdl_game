// object_types.hpp

#pragma once

#include "abstract_object.hpp"

namespace bombino {
enum object_type {
  error_value,
  user_type = levi::object_type::user_type,
  gamer1,
  gamer2,
  void_block,
  soft_block,
  solid_block,
  bomb
};
};
