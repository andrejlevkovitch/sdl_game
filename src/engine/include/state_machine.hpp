// state_machine.hpp

#pragma once

#include <list>
#include <memory>

namespace levi {
class engine;
class scene;

class state_machine {
public:
  state_machine();
  ~state_machine();
  void push_state(std::shared_ptr<scene> scene);
  /**\brief this method safely delete current state, when it not used*/
  void pop_state();
  void update();
  /**\except if current_state absent*/
  std::shared_ptr<scene> current_state();
  const std::list<std::shared_ptr<scene>> &get_states_list() const;
  void render(engine &engine) const;
  void calculate_light(engine &engine) const;

private:
  std::list<std::shared_ptr<scene>> states_list_;
  std::list<std::shared_ptr<scene>>::iterator current_state_;
};
}; // namespace levi
