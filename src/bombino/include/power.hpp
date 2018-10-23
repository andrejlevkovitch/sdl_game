// power.hpp

#pragma once

#include "abstract_object.hpp"

#include <cstdint>

#include "object_types.hpp"

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
