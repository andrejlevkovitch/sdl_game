// tile.hpp

#pragma once

#include "abstract_object.hpp"
#include "object_types.hpp"

namespace levi {
class scene;
};

namespace bombino {
class tile : public levi::abstract_object {
public:
  tile(const std::string &texture_id, levi::size size, levi::vector2d pos,
       bombino::object_type type, unsigned frame);
  void update() override;
  levi::object_type type() const override;
  void destroy();

public:
  levi::scene *scene;

private:
  object_type type_;
  uint8_t power_shance_;
};
}; // namespace bombino
