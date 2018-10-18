// pause_state.hpp

#pragma once

#include "scene.hpp"

namespace bombino {
class pause_state : public levi::scene {
public:
  pause_state();
  void update() override;
  levi::id_state get_id() const override;

private:
  levi::item_list::iterator current_;
};
}; // namespace bombino
