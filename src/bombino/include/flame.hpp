// flame.hpp

#pragma once

#include "abstract_object.hpp"

namespace bombino {
class flame : public levi::abstract_object {
public:
  flame(const std::string &texture_id, levi::size size, levi::vector2d pos);
  void update() override;

private:
  unsigned last_time_;
  unsigned cur_frame_;
};
}; // namespace bombino
