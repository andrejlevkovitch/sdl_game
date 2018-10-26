// pause_state.cpp

#include "pause_state.hpp"

#include "button.hpp"
#include "callback_map.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "objects_config.hpp"
#include "player.hpp"
#include "playing_state.hpp"

bombino::pause_state::pause_state() : scene{} {
  levi::player::instance().play("fon", true);

  auto new_items = parse_state(bombino::way_to_objects + "bombino_states.xml",
                               "pause", &callback_map::instance());
  for (auto &i : new_items) {
    item_list_.push_back(i);
  }

  current_ = item_list_.begin();
  reinterpret_cast<levi::button *>((*current_).get())->hover(true);
}

void bombino::pause_state::update() {
  scene::update();
  auto events = levi::input_handler::instance().get_event_list();
  for (auto &i : events) {
    if (i.type == levi::event_type::button_event &&
        i.button.state == levi::button_state::pressed) {
      switch (i.button.code) {
      case levi::button_code::down:
        if (current_ != --item_list_.end()) {
          auto p = dynamic_cast<levi::button *>(current_->get());
          p->hover(false);
          ++current_;
          p = dynamic_cast<levi::button *>(current_->get());
          p->hover(true);
        }
        return;
      case levi::button_code::up:
        if (current_ != item_list_.begin()) {
          auto p = dynamic_cast<levi::button *>(current_->get());
          p->hover(false);
          --current_;
          p = dynamic_cast<levi::button *>(current_->get());
          p->hover(true);
        }
        return;
      default:
        break;
      }
    }
  }
}

levi::id_state bombino::pause_state::get_id() const {
  return levi::id_state::pause;
}
