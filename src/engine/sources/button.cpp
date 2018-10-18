// button.cpp

#include "button.hpp"

#include "input_handler.hpp"

levi ::button::button(const std::string &texture_id, levi::size size,
                      levi::vector2d pos, callback callback)
    : levi::abstract_object{texture_id, size, pos}, callback_{callback},
      state_{false} {}

void levi::button::hover(bool state) {
  state_ = state;
  src_rect_.x = state * src_rect_.width;
}

void levi::button::push() { callback_(); }

void levi::button::update() {
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
