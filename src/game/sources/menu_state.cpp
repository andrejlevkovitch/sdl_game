// menu_state.cpp

#include "menu_state.hpp"

#include "button.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "playing_state.hpp"
#include "state_machine.hpp"

deep_space::menu_state::menu_state() : levi::scene{} {
  auto callback_play = []() {
    levi::engine::instance().state_machine().push_state(
        std::make_shared<playing_state>());
  };
  auto play_button = std::make_shared<deep_space::button>(
      levi::way_to_images + "play.png", callback_play);
  play_button->set_size(levi::size{400, 100});
  play_button->set_pos(levi::vector2d{100, 100});
  play_button->hover(true);

  auto callback_exit = []() {
    levi::event event{};
    event.type = levi::event_type::quit_event;
    levi::input_handler::instance().add_event(event);
  };
  auto exit_button = std::make_shared<deep_space::button>(
      levi::way_to_images + "exit.png", callback_exit);
  exit_button->set_size(levi::size{400, 100});
  exit_button->set_pos(levi::vector2d{100, 300});

  this->item_list_.push_back(play_button);
  this->item_list_.push_back(exit_button);

  current_ = item_list_.begin();
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
