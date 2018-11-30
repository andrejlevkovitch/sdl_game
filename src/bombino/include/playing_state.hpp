// playing_state.hpp

#pragma once

#include "scene.hpp"

namespace bombino {
class playing_state : public levi::scene {
public:
  playing_state();
  void update() override;
  levi::id_state get_id() const override;
  void reload() override;

private:
  int n_start_;
};
}; // namespace bombino
