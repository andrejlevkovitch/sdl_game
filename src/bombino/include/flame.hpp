// flame.hpp

#pragma once

#include "abstract_object.hpp"
#include <cstdint>

namespace bombino {
class flame : public levi::abstract_object {
public:
  flame(const std::string &texture_id, levi::size size, levi::vector2d pos,
        uint8_t frame_count);
  void update() override;

private:
  unsigned last_time_;
  unsigned cur_frame_;
  uint8_t frame_count_;
};
}; // namespace bombino
