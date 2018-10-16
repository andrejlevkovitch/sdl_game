// pause_state.cpp

#include "pause_state.hpp"

#include "button.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "playing_state.hpp"
#include "state_machine.hpp"

deep_space::pause_state::pause_state() : scene{} {
  auto resume_callback = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        true);
  };
  auto to_menu_callback = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        true);
    levi::engine::instance().state_machine().current_state()->set_visibility(
        true);
  };
  auto restart_callback = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().push_state(
        std::make_shared<playing_state>());
  };

  callback_map callback_map{};
  callback_map["resume"] = resume_callback;
  callback_map["to_menu"] = to_menu_callback;
  callback_map["restart"] = restart_callback;

  parse_state(deep_space::way_to_objects + "objects.xml", "pause", item_list_,
              &callback_map);

  current_ = item_list_.begin();
  reinterpret_cast<button *>((*current_).get())->hover(true);
}

void deep_space::pause_state::update() {
  scene::update();
  auto events = levi::input_handler::instance().get_event_list();
  for (auto &i : events) {
    if (i.type == levi::event_type::button_event &&
        i.button.state == levi::button_state::pressed) {
      switch (i.button.code) {
      case levi::button_code::down:
        if (current_ != --item_list_.end()) {
          auto p = dynamic_cast<deep_space::button *>(current_->get());
          p->hover(false);
          ++current_;
          p = dynamic_cast<deep_space::button *>(current_->get());
          p->hover(true);
        }
        return;
      case levi::button_code::up:
        if (current_ != item_list_.begin()) {
          auto p = dynamic_cast<deep_space::button *>(current_->get());
          p->hover(false);
          --current_;
          p = dynamic_cast<deep_space::button *>(current_->get());
          p->hover(true);
        }
        return;
      default:
        break;
      }
    }
  }
}

levi::id_state deep_space::pause_state::get_id() const {
  return levi::id_state::pause;
}