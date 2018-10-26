// menu_state.cpp

#include "menu_state.hpp"

#include "button.hpp"
#include "callback_map.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "object_manager.hpp"
#include "objects_config.hpp"
#include "player.hpp"

bombino::menu_state::menu_state() : levi::scene{} {
  auto &player = levi::player::instance();
  player.play("fon", true);
  player.pause(false);

  object_manager::instance().parse_file(bombino::way_to_objects +
                                        "bombino_objects.xml");

  auto new_items = parse_state(bombino::way_to_objects + "bombino_states.xml",
                               "menu", &callback_map::instance());
  for (auto &i : new_items) {
    item_list_.push_back(i);
  }

  current_ = item_list_.begin();
  reinterpret_cast<levi::button *>((*current_).get())->hover(true);
}

void bombino::menu_state::update() {
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

levi::id_state bombino::menu_state::get_id() const {
  return levi::id_state::menu;
}
