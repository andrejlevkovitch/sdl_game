// gamer.cpp

#include "gamer.hpp"

#include <memory>

#include "bomb.hpp"
#include "input_handler.hpp"
#include "objects_config.hpp"
#include "scene.hpp"
#include "time.hpp"

// TODO this game dependes on fps - I have to fixed this!
// TODO when gamer push 2 buttons, and relesed one, then character can see on
// uncorrect side
// TODO hardkod
bombino::gamer::gamer(const std::string &texture_id, levi::size size,
                      levi::vector2d pos, object_type type)
    : levi::abstract_object{texture_id, size, pos}, texture_size_{},
      distance_{}, direction_{0, 0}, velocity_{2}, front_frame_collection_{},
      side_frame_collection_{}, back_frame_collection_{},
      time_last_bomb_(-4000), time_to_new_bomb_{4000}, type_{type},
      cur_frame_{} {
  need_collisions_flag_ = true;
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

void bombino::gamer::specify_frame_collection(frames front, frames side,
                                              frames back) {
  front_frame_collection_ = front;
  side_frame_collection_ = side;
  back_frame_collection_ = back;
  cur_frame_collection_ = front_frame_collection_;
}

void bombino::gamer::set_frame(unsigned frame) {
  unsigned absolute_width = src_rect_.width * frame;
  src_rect_.y = absolute_width / texture_size_.width * src_rect_.height;
  src_rect_.x = absolute_width % texture_size_.width;
  cur_frame_ = frame;
}

void bombino::gamer::set_frame_collection(frames cur_collection) {
  cur_frame_collection_ = cur_collection;
}

void bombino::gamer::next_frame() {
  set_frame((cur_frame_ + 1) % cur_frame_collection_.second +
            cur_frame_collection_.first);
}

void bombino::gamer::set_texture_size(levi::size tex_size) {
  texture_size_ = tex_size;
}

void bombino::gamer::update() {
  auto &event_list = levi::input_handler::instance().get_event_list();
  for (auto &i : event_list) {
    if (i.type == levi::event_type::button_event) {
      if (i.button.code == active_buttons_[0]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.y = -1;
          flip_ = levi::flip::none;
          set_frame_collection(back_frame_collection_);
        } else {
          direction_.y = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[1]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.y = 1;
          flip_ = levi::flip::none;
          set_frame_collection(front_frame_collection_);
        } else {
          direction_.y = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[2]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.x = -1;
          flip_ = levi::flip::horizontal;
          set_frame_collection(side_frame_collection_);
        } else {
          direction_.x = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[3]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.x = 1;
          flip_ = levi::flip::none;
          set_frame_collection(side_frame_collection_);
        } else {
          direction_.x = 0;
        }
        continue;
      }
      if (i.button.code == active_buttons_[4]) {
        if (i.button.state == levi::button_state::pressed) {
          if (levi::get_time() > time_last_bomb_ + time_to_new_bomb_) {
            time_last_bomb_ = levi::get_time();
            levi::vector2d center_gamer{get_pos()};
            center_gamer.x += get_size().width / 2;
            center_gamer.y += get_size().height / 2;
            for (auto &collision : collisions_) {
              if (collision->type() ==
                      static_cast<levi::object_type>(object_type::void_block) &&
                  collision->get_rectangle().is_intake_pos(center_gamer)) {
                scene->add_item(std::make_shared<class bomb>(
                    "bomb", levi::size{48, 48},
                    levi::vector2d{collision->get_pos() + levi::vector2d{8, 8}},
                    1));
                break;
              }
            }
          }
        }
        continue;
      }
    }
  }
  motion();
}

void bombino::gamer::motion() {
  if (direction_ == levi::vector2d{0, 0}) {
    return;
  } else {
    distance_ = velocity_ * direction_.get_norm();
    this->set_pos(this->get_pos() + distance_);
    next_frame();
  }
}

levi::object_type bombino::gamer::type() const {
  return static_cast<levi::object_type>(type_);
}

levi::rect bombino::gamer::get_rectangle() const {
  auto retval = levi::abstract_object::get_rectangle();
  retval.x += 9;
  retval.y += 12;
  retval.width -= 16;
  retval.height -= 16;
  return retval;
}

void bombino::gamer::collision_handler() {
  for (const auto &i : collisions_) {
    if (i->type() == static_cast<levi::object_type>(object_type::soft_block) ||
        i->type() == static_cast<levi::object_type>(object_type::solid_block)) {
      this->set_pos(this->get_pos() - distance_);
      distance_ = levi::vector2d{0, 0};
      return;
    }
  }
}

void bombino::gamer::kill() { velocity_ = 0; }
