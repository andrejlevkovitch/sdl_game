// playing_state.hpp

#pragma once

#include "scene.hpp"

namespace bombino {
class playing_state : public levi::scene {
public:
  playing_state();
  void add_item(std::shared_ptr<levi::abstract_object> obj) override;
  void update() override;
  levi::id_state get_id() const override;
};
}; // namespace bombino
