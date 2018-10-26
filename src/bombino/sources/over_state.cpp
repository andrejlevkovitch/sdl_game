// over_state.cpp

#include "over_state.hpp"

#include "button.hpp"
#include "engine.hpp"
#include "object_factory.hpp"
#include "objects_config.hpp"
#include "state_machine.hpp"

bombino::over_state::over_state() {
  auto callback_to_menu = []() {
    auto &s_machine = levi::engine::instance().state_machine();
    s_machine.pop_state();
    s_machine.pop_state();
    s_machine.current_state()->set_updatebility(true);
    s_machine.current_state()->set_visibility(true);
  };

  callback_map call_map{std::make_pair("to_menu", callback_to_menu)};

  for (auto &i : parse_state(bombino::way_to_objects + "bombino_states.xml",
                             "over", &call_map)) {
    item_list_.push_back(i);
  }
  if (item_list_.empty()) {
    throw std::runtime_error{"state wasn't loaded"};
  }
  reinterpret_cast<levi::button *>(item_list_.front().get())->hover(true);
}

levi::id_state bombino::over_state::get_id() const {
  return levi::id_state::over;
}
