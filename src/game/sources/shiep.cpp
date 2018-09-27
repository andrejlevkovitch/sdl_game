// shiep.cpp

#include "shiep.hpp"
#include "config.hpp"
#include "input_handler.hpp"

static const int max_speed{30};

deep_space::shiep::shiep()
    : abstract_object{levi::way_to_images + "shiep.png"}, breaking_{0.3},
      acceleration_{2}, direction_{1, 0} {
  this->set_size(levi::size{64, 64});
  this->set_pos(levi::vector2d{100, 0});
}

void deep_space::shiep::update() {
  set_frame(0);
  auto &event_list = levi::input_handler::instance().get_event_list();
  for (auto &i : event_list) {
    if (i.type == levi::event_type::button_event) {
      switch (i.button.code) {
      case levi::button_code::up:
        if (i.button.state == levi::button_state::pressed) {
          set_frame(1);
          if (velocity_.get_length() < max_speed) {
            velocity_ += acceleration_ * direction_;
          }
        } else {
          set_frame(0);
        }
        break;
      case levi::button_code::down:
        break;
      case levi::button_code::left:
        if (i.button.state == levi::button_state::pressed) {
          this->rotate(-5);
        }
        break;
      case levi::button_code::right:
        if (i.button.state == levi::button_state::pressed) {
          this->rotate(5);
        }
        break;
      default:
        break;
      }
    }
  }
  motion();
}

void deep_space::shiep::motion() {
  this->set_pos(this->get_pos() + velocity_);
  auto distance = velocity_.get_length();
  if (distance > breaking_) {
    auto dir_velo = velocity_.get_norm();
    auto b = breaking_ * dir_velo;
    velocity_ -= b;
  } else if (distance > 0) {
    velocity_ = levi::vector2d{0, 0};
  }
}

float deep_space::shiep::rotate(float value) {
  levi::abstract_object::rotate(value);
  direction_ = direction_.get_vec_from(1, get_angle());
  return get_angle();
}

void deep_space::shiep::set_angle(float angle) {
  levi::abstract_object::set_angle(angle);
  direction_ = direction_.get_vec_from(1, angle);
}
