// light.hpp

#pragma once

#include "abstract_object.hpp"

namespace levi {
class light final : private abstract_object {
public:
  light(vector2d pos, unsigned short power);
  object_type type() const override;
  void set_pos(vector2d pos) override;
  levi::vector2d get_pos() const override;
  void set_power(unsigned short power);
  unsigned short get_power() const;
  void delete_later() override;
  bool is_for_delete() override;
  void set_scene(scene *scene) override;
  void draw(engine &engine) const override;

private:
  void update() override;
};
}; // namespace levi
