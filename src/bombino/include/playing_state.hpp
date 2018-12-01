// playing_state.hpp

#pragma once

#include "scene.hpp"
#include <vector>

namespace bombino {
class playing_state : public levi::scene {
public:
  playing_state();
  void update() override;
  levi::id_state get_id() const override;
  /**\except trow error if no one map loaded*/
  void reload() override;

private:
  int n_start_;
  std::vector<std::string> maps_;
};
}; // namespace bombino
