// gamer.hpp

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "abstract_object.hpp"
#include "event.hpp"
#include "vector2d.hpp"

namespace bombino {
enum object_type { gamer1 = levi::object_type::user_type, gamer2 };

/**\brief first is first frame, second is size of frames*/
using frames = std::pair<uint8_t, uint8_t>;

class gamer : public levi::abstract_object {
public:
  gamer(const std::string &texture_id, levi::size size, levi::vector2d pos);
  void specify_frames(frames front, frames side, frames back);
  void set_frame(unsigned frame);
  void next_frame(frames cur_frame);
  void set_texture_size(levi::size texture_size);
  void update() override;
  void motion();
  void set_angle(float angle) override;
  levi::object_type type() const override;
  void set_type(object_type);

private:
  float breaking_;
  float acceleration_;
  levi::vector2d velocity_;
  /**\brief normilize vector*/
  levi::vector2d direction_;
  unsigned cur_max_speed;

  uint8_t cur_frame_;
  frames front_frames_;
  frames side_frames_;
  frames back_frames_;
  levi::size texture_size_;

  object_type type_;

  std::vector<levi::button_code> active_buttons_;
};
}; // namespace bombino
