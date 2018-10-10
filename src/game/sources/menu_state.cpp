// menu_state.cpp

#include "menu_state.hpp"

#include "button.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "playing_state.hpp"
#include "state_machine.hpp"

#include <iostream>

deep_space::menu_state::menu_state() : levi::scene{} {
  auto callback_play = []() {
    auto cur_scene = levi::engine::instance().state_machine().current_state();
    cur_scene->set_visibility(false);
    cur_scene->set_updatebility(false);

    auto new_scene = std::make_shared<playing_state>();
    levi::engine::instance().state_machine().push_state(new_scene);
  };

  auto callback_exit = []() {
    levi::event event{};
    event.type = levi::event_type::quit_event;
    levi::input_handler::instance().add_event(event);
  };

  callback_map callback_map;

  callback_map["play"] = callback_play;
  callback_map["exit"] = callback_exit;

  parse_state(deep_space::way_to_objects + "objects.xml", "menu", item_list_,
              &callback_map);

  current_ = item_list_.begin();
  reinterpret_cast<button *>((*current_).get())->hover(true);
}

void deep_space::menu_state::update() {
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

levi::id_state deep_space::menu_state::get_id() const {
  return levi::id_state::menu;
}
