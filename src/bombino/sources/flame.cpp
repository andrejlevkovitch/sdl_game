// flame.cpp

#include "flame.hpp"
#include "time.hpp"

const unsigned frame_time{100};

bombino::flame::flame(const std::string &texture_id, levi::size size,
                      levi::vector2d pos, uint8_t frame_count)
    : levi::abstract_object{texture_id, size, pos},
      last_time_{levi::get_time()}, cur_frame_{}, frame_count_{frame_count} {}

void bombino::flame::update() {
  if (levi::get_time() >= (last_time_ + frame_time)) {
    ++cur_frame_;
    if (cur_frame_ >= frame_count_) {
      this->delete_later();
      return;
    }
    src_rect_.x += src_rect_.width;
    last_time_ = levi::get_time();
  }
}
