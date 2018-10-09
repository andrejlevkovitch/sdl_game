// abstract_object.cpp

#include "abstract_object.hpp"

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

levi::abstract_object::abstract_object(params params)
    : file_name_{std::get<0>(params)},
      wait_delete_{false}, texture_{nullptr}, src_rect_{new ::SDL_Rect},
      dst_rect_{new ::SDL_Rect}, frame_{}, angle_{}, flip_{flip::none} {
  src_rect_->w = dst_rect_->w = std::get<1>(params).get_width();
  src_rect_->h = dst_rect_->h = std::get<1>(params).get_height();
  src_rect_->x = 0;
  src_rect_->y = 0;
  dst_rect_->x = std::get<2>(params).get_x();
  dst_rect_->y = std::get<2>(params).get_y();
}

levi::abstract_object::abstract_object(const std::string &file_name)
    : file_name_{file_name},
      wait_delete_{false}, texture_{nullptr}, src_rect_{new ::SDL_Rect},
      dst_rect_{new ::SDL_Rect}, frame_{}, angle_{}, flip_{flip::none} {
  dst_rect_->x = src_rect_->x = 0;
  dst_rect_->y = src_rect_->y = 0;
  dst_rect_->w = src_rect_->w = 0;
  dst_rect_->h = src_rect_->h = 0;
}

levi::abstract_object::~abstract_object() {
  ::SDL_DestroyTexture(texture_);
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

bool levi::load(::SDL_Renderer *renderer, abstract_object *obj) {
  if (obj->texture_) {
    ::SDL_DestroyTexture(obj->texture_);
  }
  auto surface = ::IMG_Load(obj->file_name_.c_str());
  if (surface) {
    obj->texture_ = ::SDL_CreateTextureFromSurface(renderer, surface);
    ::SDL_FreeSurface(surface);
    if (obj->texture_) {
      return true;
    }
  }
  return false;
}

void levi::draw(::SDL_Renderer *renderer, abstract_object *obj) {
  if (!obj->texture_) {
    if (!load(renderer, obj)) {
      obj->delete_later();
      return;
    }
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
  ::SDL_RenderCopyEx(renderer, obj->texture_, obj->src_rect_, obj->dst_rect_,
                     obj->angle_, nullptr, f);
}
