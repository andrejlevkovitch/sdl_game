// light.hpp

#pragma once

#include "abstract_object.hpp"

namespace levi {
class light final : private abstract_object {
public:
  light(vector2d pos, unsigned short power, vertex color = vertex{1, 1, 1});
  object_type type() const override;
  void set_pos(vector2d pos) override;
  levi::vector2d get_pos() const override;
  unsigned short get_power() const;
  vertex get_color() const;
  void set_color(vertex color);
  void set_power(unsigned short power);
  void delete_later() override;
  bool is_for_delete() override;
  void set_scene(scene *scene) override;
  void draw(engine &engine) override;

private:
  void update() override;
  vertex color_;
};
}; // namespace levi
