// shiep.cpp

#include "shiep.hpp"
#include "config.hpp"
#include "input_handler.hpp"

static const int max_speed{30};

deep_space::shiep::shiep(const std::string &file_name, levi::size size,
                         levi::vector2d pos)
    : levi::abstract_object{file_name, size, pos}, breaking_{0.3},
      acceleration_{}, direction_{1, 0} {}

void deep_space::shiep::update() {
  auto &event_list = levi::input_handler::instance().get_event_list();
  for (auto &i : event_list) {
    if (i.type == levi::event_type::button_event) {
      switch (i.button.code) {
      case levi::button_code::up:
        if (i.button.state == levi::button_state::pressed) {
          acceleration_ = 2;
          set_frame(1);
        } else {
          acceleration_ = 0;
          set_frame(0);
        }
        break;
      case levi::button_code::down:
        break;
      case levi::button_code::left:
        if (i.button.state == levi::button_state::pressed) {
          rotation_ = -5;
        } else {
          rotation_ = 0;
        }
        break;
      case levi::button_code::right:
        if (i.button.state == levi::button_state::pressed) {
          rotation_ = 5;
        } else {
          rotation_ = 0;
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
  if (velocity_.get_length() < max_speed) {
    velocity_ += acceleration_ * direction_;
  }
  if (rotation_) {
    this->rotate(rotation_);
  }
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
