// power.hpp

#pragma once

#include "abstract_object.hpp"
#include "object_types.hpp"
#include <cstdint>

namespace bombino {
class power : public levi::abstract_object {
public:
  power(const std::string &texture_id, levi::size size, levi::vector2d pos,
        powers p, uint8_t frame);
  void update() override;
  levi::object_type type() const override;
  powers get_power() const;

private:
  powers power_;
};
}; // namespace bombino
