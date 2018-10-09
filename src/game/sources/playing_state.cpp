// playing_state.cpp

#include "playing_state.hpp"

#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "pause_state.hpp"
#include "shiep.hpp"
#include "state_machine.hpp"

const int dop_height{40};
const int dop_width{40};

deep_space::playing_state::playing_state() {
  parse_state(levi::way_to_collection + "collection.xml", "playing",
              item_list_);
}

void deep_space::playing_state::update() {
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
    if (item_pos.get_x() < -dop_width) {
      i->set_pos(
          levi::vector2d(win_size.get_width() + dop_width, item_pos.get_y()));
    }
    if (item_pos.get_y() < -dop_height) {
      i->set_pos(
          levi::vector2d(item_pos.get_x(), win_size.get_height() + dop_height));
    }
    if (item_pos.get_x() > win_size.get_width() + dop_width) {
      i->set_pos(levi::vector2d(-dop_width, item_pos.get_y()));
    }
    if (item_pos.get_y() > win_size.get_height() + dop_height) {
      i->set_pos(levi::vector2d(item_pos.get_x(), -dop_width));
    }
  }
}

levi::id_state deep_space::playing_state::get_id() const {
  return levi::id_state::playing;
}
