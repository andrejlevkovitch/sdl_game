// tile.hpp

#pragma once

#include "abstract_object.hpp"
#include "object_types.hpp"

namespace bombino {
class tile : public levi::abstract_object {
public:
  tile(const std::string &texture_id, levi::size size, levi::vector2d pos,
       bombino::object_type type, unsigned frame);
  void update() override;
  levi::object_type type() const override;
  void destroy();

private:
  object_type type_;
};
}; // namespace bombino
