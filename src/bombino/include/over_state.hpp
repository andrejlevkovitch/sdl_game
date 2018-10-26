// over_state.hpp

#pragma once

#include "scene.hpp"

namespace bombino {
class over_state : public levi::scene {
public:
  over_state();
  levi::id_state get_id() const override;

private:
};
}; // namespace bombino
