// gamer.hpp

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "abstract_object.hpp"
#include "event.hpp"
#include "object_types.hpp"
#include "vector2d.hpp"

namespace bombino {
/**\brief first is first frame, second is size of frames*/
using frames = std::pair<uint8_t, uint8_t>;

class gamer : public levi::abstract_object {
public:
  gamer(const std::string &texture_id, levi::size size, levi::vector2d pos,
        bombino::object_type type);
  /**\brief this method needed for load two-dimensional sprites.
   * \param side right side frames, for left side using flip*/
  void specify_frame_collection(frames front, frames side, frames back);
  void set_frame(unsigned frame);
  void set_frame_collection(frames cur_collection);
  /**\brief set next frame, using current collection of frames*/
  void next_frame();
  /**\brief this function needed for load two-dimensional sprites*/
  void set_texture_size(levi::size texture_size);
  void update() override;
  void motion();
  levi::object_type type() const override;
  levi::rect get_rectangle() const override;
  void collision_handler() override;

private:
  float acceleration_;
  levi::vector2d velocity_;
  /**\brief normilize vector*/
  levi::vector2d direction_;

  uint8_t cur_frame_;
  frames front_frame_collection_;
  frames side_frame_collection_;
  frames back_frame_collection_;
  frames cur_frame_collection_;
  levi::size texture_size_;

  object_type type_;

  std::vector<levi::button_code> active_buttons_;
};
}; // namespace bombino
