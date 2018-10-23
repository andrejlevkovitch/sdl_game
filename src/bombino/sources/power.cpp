// power.cpp

#include "power.hpp"

bombino::power::power(const std::string &texture_id, levi::size size,
                      levi::vector2d pos, powers p, uint8_t frame)
    : levi::abstract_object{texture_id, size, pos}, power_{p} {
  src_rect_.x = frame * src_rect_.width;
}

void bombino::power::update() {}

levi::object_type bombino::power::type() const {
  return static_cast<levi::object_type>(object_type::power);
}

bombino::powers bombino::power::get_power() const { return power_; }
