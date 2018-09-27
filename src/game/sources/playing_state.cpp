// playing_state.cpp

#include "playing_state.hpp"

#include "engine.hpp"
#include "input_handler.hpp"
#include "pause_state.hpp"
#include "shiep.hpp"
#include "state_machine.hpp"

deep_space::playing_state::playing_state() {
  this->add_item(std::make_shared<shiep>());
}

void deep_space::playing_state::update() {
  auto events = levi::input_handler::instance().get_event_list();
  for (auto &i : events) {
    if (i.type == levi::event_type::pause_event) {
      levi::engine::instance().state_machine().push_state(
          std::make_shared<pause_state>());
      return;
    }
  }

  levi::scene::update();

  auto win_size = levi::engine::instance().get_window_size();
  for (auto &i : item_list_) {
    auto item_pos = i->get_pos();
    if (item_pos.get_x() < -40) {
      i->set_pos(levi::vector2d(win_size.get_width() + 40, item_pos.get_y()));
    }
    if (item_pos.get_y() < -40) {
      i->set_pos(levi::vector2d(item_pos.get_x(), win_size.get_height() + 40));
    }
    if (item_pos.get_x() > win_size.get_width() + 40) {
      i->set_pos(levi::vector2d(-40, item_pos.get_y()));
    }
    if (item_pos.get_y() > win_size.get_height() + 40) {
      i->set_pos(levi::vector2d(item_pos.get_x(), -40));
    }
  }
}

levi::id_state deep_space::playing_state::get_id() const {
  return levi::id_state::playing;
}
