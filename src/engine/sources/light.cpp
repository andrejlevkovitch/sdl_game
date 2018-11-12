// light.cpp

#include "light.hpp"
#include "engine.hpp"
#include "texture_manager.hpp"

levi::light::light(vector2d pos, unsigned short power, vertex color)
    : abstract_object{"circle", size{64, 64},
                      vector2d{pos.x - power, pos.y - power}},
      color_{color} {
  dst_rect_.width = power * 2;
  dst_rect_.height = power * 2;
}

levi::object_type levi::light::type() const { return object_type::light; }

void levi::light::set_pos(vector2d pos) {
  abstract_object::set_pos(
      vector2d{pos.x - dst_rect_.width / 2, pos.y - dst_rect_.height / 2});
}

levi::vector2d levi::light::get_pos() const {
  return abstract_object::get_pos() +
         vector2d(dst_rect_.width / 2, dst_rect_.height / 2);
}

void levi::light::set_power(unsigned short power) {
  dst_rect_.width = power * 2;
  dst_rect_.height = power * 2;
}

unsigned short levi::light::get_power() const { return dst_rect_.width / 2; }

void levi::light::delete_later() { abstract_object::delete_later(); }

bool levi::light::is_for_delete() { return abstract_object::is_for_delete(); }

void levi::light::set_scene(scene *scene) { abstract_object::set_scene(scene); }

void levi::light::draw(engine &engine) {
  texture texture;
  try {
    texture = engine.texture_manager().get_texture(texture_id_);
  } catch (std::exception &) {
    return;
  }
  engine.draw_light(texture, src_rect_, dst_rect_, color_);
}

void levi::light::update() {}

levi::vertex levi::light::get_color() const { return color_; }
void levi::light::set_color(vertex color) { color_ = color; }
