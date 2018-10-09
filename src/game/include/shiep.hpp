// shiep.hpp

#pragma once

#include "abstract_object.hpp"
#include "vector2d.hpp"

namespace deep_space {
class shiep : public levi::abstract_object {
public:
  shiep(const std::string &file_name, levi::size size, levi::vector2d pos);
  void update() override;
  void motion();
  float rotate(float value) override;
  void set_angle(float angle) override;

private:
  float breaking_;
  float acceleration_;
  float rotation_;
  levi::vector2d velocity_;
  /**\brief normilize vector*/
  levi::vector2d direction_;
};
}; // namespace deep_space
