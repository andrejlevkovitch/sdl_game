// pause_state.cpp

#include "pause_state.hpp"

#include "button.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "playing_state.hpp"
#include "state_machine.hpp"

deep_space::pause_state::pause_state() : scene{} {
  auto resume_callback = []() {
    levi::engine::instance().state_machine().pop_state();
  };
  auto to_menu_callback = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().pop_state();
  };
  auto restart_callback = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().push_state(
        std::make_shared<playing_state>());
  };

  auto resume_button = std::make_shared<deep_space::button>(
      levi::way_to_images + "resume.png", resume_callback);
  resume_button->set_size(levi::size{200, 80});
  resume_button->set_pos(levi::vector2d{200, 50});
  resume_button->hover(true);

  auto to_menu_button = std::make_shared<deep_space::button>(
      levi::way_to_images + "to_menu.png", to_menu_callback);
  to_menu_button->set_size(levi::size{200, 80});
  to_menu_button->set_pos(levi::vector2d{200, 150});

  auto restart_button = std::make_shared<deep_space::button>(
      levi::way_to_images + "restart.png", restart_callback);
  restart_button->set_size(levi::size{200, 80});
  restart_button->set_pos(levi::vector2d{200, 250});

  item_list_.push_back(resume_button);
  item_list_.push_back(to_menu_button);
  item_list_.push_back(restart_button);

  current_ = item_list_.begin();
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
