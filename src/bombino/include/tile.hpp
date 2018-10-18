// tile.hpp

#pragma once

#include "abstract_object.hpp"
#include "object_types.hpp"

namespace bombino {
class tile : public levi::abstract_object {
public:
  tile(const std::string &texture_id, levi::size size, levi::vector2d pos);
  void update() override;
  levi::object_type type() const override;
  void destroy();
};
}; // namespace bombino
