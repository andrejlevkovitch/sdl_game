// abstract_object.cpp

#include "abstract_object.hpp"

#include <cmath>

#include <SDL2/SDL.h>

#include "engine.hpp"
#include "texture_manager.hpp"

#include <iostream>

levi::abstract_object::abstract_object(const std::string &texture_id, size size,
                                       vector2d pos)
    : texture_id_{texture_id}, wait_delete_{false}, src_rect_{new ::SDL_Rect},
      dst_rect_{new ::SDL_Rect}, frame_{}, angle_{}, flip_{flip::none} {
  src_rect_->x = 0;
  src_rect_->y = 0;
  dst_rect_->x = pos.get_x();
  dst_rect_->y = pos.get_y();
  dst_rect_->w = src_rect_->w = size.get_width();
  dst_rect_->h = src_rect_->h = size.get_height();
}

levi::abstract_object::~abstract_object() {
  delete src_rect_;
  delete dst_rect_;
}

levi::object_type levi::abstract_object::type() const {
  return object_type::unknown;
}

void levi::abstract_object::delete_later() { wait_delete_ = true; }

bool levi::abstract_object::is_for_delete() { return wait_delete_; }

void levi::abstract_object::set_size(levi::size size) {
  src_rect_->w = dst_rect_->w = size.get_width();
  src_rect_->h = dst_rect_->h = size.get_height();
}

levi::size levi::abstract_object::get_size() const {
  return levi::size{dst_rect_->w, dst_rect_->h};
}

void levi::abstract_object::set_pos(vector2d pos) {
  dst_rect_->x = pos.get_x();
  dst_rect_->y = pos.get_y();
}

levi::vector2d levi::abstract_object::get_pos() const {
  return levi::vector2d(dst_rect_->x, dst_rect_->y);
}

void levi::abstract_object::set_frame(int frame) {
  frame_ = frame;
  src_rect_->x = frame * src_rect_->w;
}

int levi::abstract_object::get_frame() const { return frame_; }

float levi::abstract_object::rotate(float angle) {
  return std::fmod(angle_ += angle, 360);
}

void levi::abstract_object::set_angle(float angle) { angle_ = angle; }

float levi::abstract_object::get_angle() const {
  return std::fmod(angle_, 360);
}

void levi::abstract_object::set_flip(flip f) { flip_ = f; }

levi::flip levi::abstract_object::get_flip() const { return flip_; }

void levi::draw(levi::engine &engine, abstract_object *obj) {
  ::SDL_Texture *texture{nullptr};
  texture = engine.texture_manager().get_texture(obj->texture_id_);
  if (!texture) {
    return;
  }
  ::SDL_RendererFlip f;
  switch (obj->flip_) {
  case flip::horizontal:
    f = ::SDL_FLIP_HORIZONTAL;
    break;
  case flip::vertical:
    f = ::SDL_FLIP_VERTICAL;
    break;
  default:
    f = ::SDL_FLIP_NONE;
    break;
  }
  ::SDL_RenderCopyEx(engine.renderer_, texture, obj->src_rect_, obj->dst_rect_,
                     obj->angle_, nullptr, f);
}
