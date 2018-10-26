// gamer.hpp

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "abstract_object.hpp"
#include "event.hpp"
#include "object_types.hpp"
#include "vector2d.hpp"

namespace levi {
class scene;
};

namespace bombino {
/**\brief first is first frame, second is end of frames (not valid!)*/
using frames = std::pair<uint8_t, uint8_t>;

class gamer : public levi::abstract_object {
public:
  /**\param callback callback function, which call when gamer was be killed*/
  gamer(const std::string &texture_id, levi::size size, levi::vector2d pos,
        bombino::object_type type, levi::callback callback);
  /**\brief this method needed for load two-dimensional sprites.
   * \param side right side frames, for left side using flip*/
  void specify_frame_collection(frames front, frames back, frames side);
  void set_frame(unsigned frame);
  void set_frame_collection(frames cur_collection);
  /**\brief set next frame, using current collection of frames*/
  void next_frame();
  /**\brief this function needed for load two-dimensional sprites*/
  void set_texture_width(unsigned texture_width);
  void update() override;
  void motion();
  levi::object_type type() const override;
  levi::rect get_rectangle() const override;
  void collision_handler();
  /**\brief call seted callback*/
  void kill();

private:
  levi::callback callback_;
  /**\brief texture size for calculate current sprite*/
  unsigned texture_width_;
  levi::vector2d distance_;
  /**\brief direction is vector with valid values for x, y ONLY 0, 1, -1. When
   * we calculate destination point, then we can get normalize vector
   * from this, but we havn't to change the vector!*/
  levi::vector2d direction_;
  /**\warning don't use values with .5 - becose it use round function!!! Better
   * use integer values*/
  float velocity_;

  /**\brief we can create two objects of this class for two gamers, and we have
   * to set different buttons for control*/
  std::vector<levi::button_code> active_buttons_;

  frames front_frame_collection_;
  frames side_frame_collection_;
  frames back_frame_collection_;
  frames cur_frame_collection_;

  uint32_t time_last_bomb_;
  uint32_t time_to_new_bomb_;

  object_type type_;
  uint8_t cur_frame_;

  uint8_t explosition_power_;
};
}; // namespace bombino
