// bomb.hpp

#pragma once

#include <cstdint>

#include "abstract_object.hpp"

namespace levi {
class scene;
};

namespace bombino {
class bomb : public levi::abstract_object {
public:
  bomb(const std::string &texture_id, levi::size size, levi::vector2d pos,
       uint8_t power);
  ~bomb();
  void update() override;
  void blow_up();
  levi::object_type type() const override;

public:
  levi::scene *scene;

private:
  uint32_t creating_time_;
  uint8_t power_;
};
}; // namespace bombino
