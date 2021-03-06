// abstract_object.cpp

#include "abstract_object.hpp"
#include "engine.hpp"
#include "texture_manager.hpp"
#include "vector2d.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

levi::abstract_object::abstract_object(const std::string &texture_id, size size,
                                       vector2d pos)
    : scene_{nullptr}, texture_id_{texture_id}, src_rect_{}, dst_rect_{},
      angle_{}, flip_{flip::none}, depth_{depth::pre_front_ground},
      wait_delete_{false} {
  src_rect_.x = 0;
  src_rect_.y = 0;
  dst_rect_.x = pos.x;
  dst_rect_.y = pos.y;
  dst_rect_.width = src_rect_.width = size.width;
  dst_rect_.height = src_rect_.height = size.height;
}

levi::abstract_object::~abstract_object() {}

levi::object_type levi::abstract_object::type() const {
  return object_type::unknown;
}

void levi::abstract_object::delete_later() { wait_delete_ = true; }

bool levi::abstract_object::is_for_delete() { return wait_delete_; }

void levi::abstract_object::set_size(levi::size size) {
  src_rect_.width = dst_rect_.width = size.width;
  src_rect_.height = dst_rect_.height = size.height;
}

levi::size levi::abstract_object::get_size() const {
  return levi::size(dst_rect_.width, dst_rect_.height);
}

void levi::abstract_object::set_pos(vector2d pos) {
  dst_rect_.x = pos.x;
  dst_rect_.y = pos.y;
}

levi::vector2d levi::abstract_object::get_pos() const {
  return levi::vector2d(dst_rect_.x, dst_rect_.y);
}

float levi::abstract_object::rotate(float angle) {
  return std::fmod(angle_ += angle, 360);
}

void levi::abstract_object::set_angle(float angle) { angle_ = angle; }

float levi::abstract_object::get_angle() const {
  return std::fmod(angle_, 360);
}

void levi::abstract_object::set_flip(flip f) { flip_ = f; }

levi::flip levi::abstract_object::get_flip() const { return flip_; }

void levi::abstract_object::draw(levi::engine &engine) {
  texture texture;
  try {
    texture = engine.texture_manager().get_texture(texture_id_);
  } catch (std::exception &) {
    return;
  }
  engine.draw(texture, src_rect_, dst_rect_, angle_, flip_, depth_);
}

levi::rect levi::abstract_object::get_rectangle() const { return dst_rect_; }

void levi::abstract_object::set_scene(scene *scene) { scene_ = scene; }
