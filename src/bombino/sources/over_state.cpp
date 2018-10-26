// over_state.cpp

#include "over_state.hpp"

#include "button.hpp"
#include "callback_map.hpp"
#include "object_factory.hpp"
#include "objects_config.hpp"

bombino::over_state::over_state() {
  for (auto &i : parse_state(bombino::way_to_objects + "bombino_states.xml",
                             "over", &callback_map::instance())) {
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
