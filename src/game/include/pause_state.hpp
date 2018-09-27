// pause_state.hpp

#include "scene.hpp"

namespace deep_space {
class pause_state : public levi::scene {
public:
  pause_state();
  void update() override;
  levi::id_state get_id() const override;

private:
  levi::item_list::iterator current_;
};
}; // namespace deep_space
