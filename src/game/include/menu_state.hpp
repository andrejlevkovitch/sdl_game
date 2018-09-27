// menu_state.hpp

#pragma once

#include "scene.hpp"

namespace deep_space {
class menu_state : public levi::scene {
public:
  menu_state();
  void update() override;
  levi::id_state get_id() const override;

private:
  levi::item_list::iterator current_;
};
}; // namespace deep_space
