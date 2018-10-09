// button.cpp

#include "button.hpp"

#include "input_handler.hpp"

deep_space::button::button(const std::string &file_name, levi::size size,
                           levi::vector2d pos, callback callback)
    : levi::abstract_object{file_name, size, pos}, callback_{callback},
      state_{false} {}

void deep_space::button::hover(bool state) {
  state_ = state;
  set_frame(state);
}

void deep_space::button::push() { callback_(); }

void deep_space::button::update() {
  if (state_) {
    auto events = levi::input_handler::instance().get_event_list();
    for (auto &i : events) {
      if (i.type == levi::event_type::button_event) {
        if (i.button.code == levi::button_code::select &&
            i.button.state == levi::button_state::pressed) {
          this->push();
          return;
        }
      }
    }
  }
}
