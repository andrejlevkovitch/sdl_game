// gamer.cpp

#include "gamer.hpp"
#include "bomb.hpp"
#include "input_handler.hpp"
#include "object_manager.hpp"
#include "objects_config.hpp"
#include "power.hpp"
#include "scene.hpp"
#include "time.hpp"
#include <memory>

namespace bombino {
const levi::vector2d up{0, -1};
const levi::vector2d down{0, 1};
const levi::vector2d right{1, 0};
const levi::vector2d left{-1, 0};
}; // namespace bombino

// TODO this game dependes on fps - I have to fixed this!
bombino::gamer::gamer(const std::string &texture_id, levi::size size,
                      levi::vector2d pos, object_type type,
                      std::function<void(void)> callback)
    : levi::abstract_object{texture_id, size, pos}, callback_{callback},
      texture_width_{}, distance_{}, direction_{0, 0}, velocity_{3},
      front_frame_collection_{}, side_frame_collection_{},
      back_frame_collection_{},
      time_last_bomb_(-4000) /*this value have to be < 0, otherwise, gamer will
                                not push bomb firt time_to_new_bomb*/
      ,
      time_to_new_bomb_{4000}, type_{type}, cur_frame_{}, explosition_power_{
                                                              1} {
  depth_ = levi::depth::front_ground;
  active_buttons_.clear();
  switch (type) {
  case object_type::gamer1:
    active_buttons_.push_back(levi::button_code::up);
    active_buttons_.push_back(levi::button_code::down);
    active_buttons_.push_back(levi::button_code::left);
    active_buttons_.push_back(levi::button_code::right);
    active_buttons_.push_back(levi::button_code::select);
    break;
  case object_type::gamer2:
    active_buttons_.push_back(levi::button_code::up_dop);
    active_buttons_.push_back(levi::button_code::down_dop);
    active_buttons_.push_back(levi::button_code::left_dop);
    active_buttons_.push_back(levi::button_code::right_dop);
    active_buttons_.push_back(levi::button_code::select_dop);
    break;
  default:
    break;
  }
}

void bombino::gamer::specify_frame_collection(frames front, frames back,
                                              frames side) {
  front_frame_collection_ = front;
  back_frame_collection_ = back;
  side_frame_collection_ = side;
  cur_frame_collection_ = front_frame_collection_;
}

void bombino::gamer::set_frame(unsigned frame) {
  unsigned absolute_width = src_rect_.width * frame;
  src_rect_.y = absolute_width / texture_width_ * src_rect_.height;
  src_rect_.x = absolute_width % texture_width_;
  cur_frame_ = frame;
}

void bombino::gamer::set_frame_collection(frames cur_collection) {
  if (cur_collection != cur_frame_collection_) {
    cur_frame_collection_ = cur_collection;
    cur_frame_ = cur_frame_collection_.first;
  }
}

void bombino::gamer::next_frame() {
  set_frame((++cur_frame_ >= cur_frame_collection_.second)
                ? cur_frame_collection_.first
                : cur_frame_);
}

void bombino::gamer::set_texture_width(unsigned tex_width) {
  texture_width_ = tex_width;
}

void bombino::gamer::update() {
  auto &event_list = levi::input_handler::instance().get_event_list();
  for (auto &i : event_list) {
    if (i.type == levi::event_type::button_event) {
      if (i.button.code == active_buttons_[0]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.y = up.y;
          flip_ = levi::flip::none;
          set_frame_collection(back_frame_collection_);
        } else if (direction_.y == up.y) {
          direction_.y = 0;
        }
        continue;
      } else if (i.button.code == active_buttons_[1]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.y = down.y;
          flip_ = levi::flip::none;
          set_frame_collection(front_frame_collection_);
        } else if (direction_.y == down.y) {
          direction_.y = 0;
        }
        continue;
      } else if (i.button.code == active_buttons_[2]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.x = left.x;
          flip_ = levi::flip::horizontal;
          set_frame_collection(side_frame_collection_);
        } else if (direction_.x == left.x) {
          direction_.x = 0;
        }
        continue;
      } else if (i.button.code == active_buttons_[3]) {
        if (i.button.state == levi::button_state::pressed) {
          direction_.x = right.x;
          flip_ = levi::flip::none;
          set_frame_collection(side_frame_collection_);
        } else if (direction_.x == right.x) {
          direction_.x = 0;
        }
        continue;
      } else if (i.button.code == active_buttons_[4]) {
        if (i.button.state == levi::button_state::pressed) {
          if (levi::get_time() > time_last_bomb_ + time_to_new_bomb_) {
            time_last_bomb_ = levi::get_time();
            // we have to find tile in which we set bomb
            levi::vector2d center_gamer{get_pos()};
            center_gamer.x += get_size().width / 2;
            center_gamer.y += get_size().height / 2;
            for (auto &collision :
                 scene_->get_collisions_for(this->get_rectangle())) {
              if (collision->type() ==
                      static_cast<levi::object_type>(object_type::void_block) &&
                  collision->get_rectangle().is_intake_pos(center_gamer)) {
                auto &bomb_params =
                    object_manager::instance().get_obj_params("bomb");
                // bomb size and tile size can (read as have to) be difference!
                levi::vector2d bomb_pos = collision->get_pos();
                bomb_pos.x += (collision->get_size().width -
                               bomb_params.object_size.width) /
                              2;
                bomb_pos.y += (collision->get_size().height -
                               bomb_params.object_size.height) /
                              2;
                scene_->add_item(std::make_shared<class bomb>(
                    bomb_params.texture_id, bomb_params.object_size, bomb_pos,
                    explosition_power_));
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
  collision_handler();
}

void bombino::gamer::motion() {
  if (!(direction_ == levi::vector2d{0, 0})) {
    distance_ = velocity_ * direction_.get_norm();
    this->set_pos(this->get_pos() + distance_);
    next_frame();
  }
}

levi::object_type bombino::gamer::type() const {
  return static_cast<levi::object_type>(type_);
}

// TODO hardkod
levi::rect bombino::gamer::get_rectangle() const {
  auto retval = levi::abstract_object::get_rectangle();
  retval.x += 10;
  retval.y += 14;
  retval.width -= 20;
  retval.height -= 20;
  return retval;
}

void bombino::gamer::collision_handler() {
  for (const auto &i : scene_->get_collisions_for(get_rectangle())) {
    switch (static_cast<object_type>(i->type())) {
    case object_type::soft_block:
    case object_type::solid_block:
      this->set_pos(this->get_pos() - distance_);
      distance_ = levi::vector2d{0, 0};
      break;
    case object_type::bomb:
      if (!reinterpret_cast<class bomb *>(i)->can_walk(this)) {
        this->set_pos(this->get_pos() - distance_);
        distance_ = levi::vector2d{0, 0};
      }
      break;
    case object_type::power:
      switch (dynamic_cast<class power *>(i)->get_power()) {
      case powers::energy:
        ++explosition_power_;
        break;
      case powers::bombs:
        time_to_new_bomb_ /= 2;
        break;
      case powers::speed:
        velocity_ += 1;
        break;
      default:
        break;
      }
      i->delete_later();
      break;
    default:
      break;
    }
  }
}

void bombino::gamer::kill() {
  velocity_ = 0;
  if (callback_ != nullptr) {
    callback_();
  }
}
