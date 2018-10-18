// gamer.cpp

#include "gamer.hpp"

#include "config.hpp"
#include "input_handler.hpp"
#include "time.hpp"

const unsigned fixed_time_update{50};

bombino::gamer::gamer(const std::string &texture_id, levi::size size,
                      levi::vector2d pos)
    : levi::abstract_object{texture_id, size, pos}, breaking_{0.5},
      acceleration_{}, direction_{1, 0}, cur_max_speed{3}, cur_frame_{},
      front_frames_{}, side_frames_{}, back_frames_{}, texture_size_{}, type_{},
      last_tick_{} {
  need_collisions_flag_ = true;
}

void bombino::gamer::specify_frames(frames front, frames side, frames back) {
  front_frames_ = front;
  side_frames_ = side;
  back_frames_ = back;
}

void bombino::gamer::set_frame(unsigned frame) {
  unsigned absolute_width = src_rect_.width * frame;
  src_rect_.y = absolute_width / texture_size_.width * src_rect_.height;
  src_rect_.x = absolute_width % texture_size_.width;
  cur_frame_ = frame;
}

void bombino::gamer::next_frame(frames cur_frames) {
  set_frame((cur_frame_ + 1) % cur_frames.second + cur_frames.first);
}

void bombino::gamer::set_texture_size(levi::size tex_size) {
  texture_size_ = tex_size;
}

void bombino::gamer::update() {
  unsigned cur_tick{levi::get_time()};
  if ((cur_tick - last_tick_) < fixed_time_update) {
    return;
  } else {
    last_tick_ = cur_tick;
  }
  auto &event_list = levi::input_handler::instance().get_event_list();
  for (auto &i : event_list) {
    if (i.type == levi::event_type::button_event) {
      if (i.button.code == active_buttons_[0]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_ = levi::vector2d{0, -1};
          acceleration_ = 2;
          set_flip(levi::flip_type::none);
          next_frame(back_frames_);
        } else {
          acceleration_ = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[1]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_ = levi::vector2d{0, 1};
          acceleration_ = 2;
          set_flip(levi::flip_type::none);
          next_frame(front_frames_);
        } else {
          acceleration_ = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[2]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_ = levi::vector2d{-1, 0};
          acceleration_ = 2;
          set_flip(levi::flip_type::horizontal);
          next_frame(side_frames_);
        } else {
          acceleration_ = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[3]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_ = levi::vector2d{1, 0};
          acceleration_ = 2;
          set_flip(levi::flip_type::none);
          next_frame(side_frames_);
        } else {
          acceleration_ = 0;
        }
        continue;
      }
    }
  }
  motion();
}

void bombino::gamer::motion() {
  if (velocity_.get_length() < cur_max_speed) {
    velocity_ += acceleration_ * direction_;
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

levi::object_type bombino::gamer::type() const {
  return static_cast<levi::object_type>(type_);
}

void bombino::gamer::set_type(object_type type) {
  type_ = type;
  active_buttons_.clear();
  if (type_ == object_type::gamer1) {
    active_buttons_.push_back(levi::button_code::up);
    active_buttons_.push_back(levi::button_code::down);
    active_buttons_.push_back(levi::button_code::left);
    active_buttons_.push_back(levi::button_code::right);
    active_buttons_.push_back(levi::button_code::select);
  }
  if (type_ == object_type::gamer2) {
    active_buttons_.push_back(levi::button_code::up_dop);
    active_buttons_.push_back(levi::button_code::down_dop);
    active_buttons_.push_back(levi::button_code::left_dop);
    active_buttons_.push_back(levi::button_code::right_dop);
    active_buttons_.push_back(levi::button_code::select_dop);
  }
}

levi::rect bombino::gamer::get_rectangle() const {
  auto retval = levi::abstract_object::get_rectangle();
  retval.x += 9;
  retval.y += 12;
  retval.width -= 16;
  retval.height -= 16;
  return retval;
}
