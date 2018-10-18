// playing_state.cpp

#include "playing_state.hpp"

#include "config.hpp"
#include "engine.hpp"
#include "gamer.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "pause_state.hpp"
#include "state_machine.hpp"

const int dop_height{40};
const int dop_width{40};

bombino::playing_state::playing_state() {
  parse_state(bombino::way_to_objects + "bombino_states.xml", "playing",
              item_list_);
}

void bombino::playing_state::update() {
  auto events = levi::input_handler::instance().get_event_list();
  for (auto &i : events) {
    if (i.type == levi::event_type::pause_event) {
      levi::engine::instance()
          .state_machine()
          .current_state()
          ->set_updatebility(false);
      levi::engine::instance().state_machine().push_state(
          std::make_shared<pause_state>());
      return;
    }
  }

  levi::scene::update();

  auto win_size = levi::engine::instance().get_window_size();
  for (auto &i : item_list_) {
    auto item_pos = i->get_pos();
    if (item_pos.x < -dop_width) {
      i->set_pos(levi::vector2d(win_size.width + dop_width, item_pos.y));
    }
    if (item_pos.y < -dop_height) {
      i->set_pos(levi::vector2d(item_pos.x, win_size.height + dop_height));
    }
    if (item_pos.x > win_size.width + dop_width) {
      i->set_pos(levi::vector2d(-dop_width, item_pos.y));
    }
    if (item_pos.y > win_size.height + dop_height) {
      i->set_pos(levi::vector2d(item_pos.x, -dop_width));
    }
  }
}

levi::id_state bombino::playing_state::get_id() const {
  return levi::id_state::playing;
}
