// flame.cpp

#include "flame.hpp"
#include "time.hpp"

#include <iostream>

const unsigned frame_time{100};

bombino::flame::flame(const std::string &texture_id, levi::size size,
                      levi::vector2d pos)
    : levi::abstract_object{texture_id, size, pos}, last_time_{
                                                        levi::get_time()} {
  std::cerr << "FLAME\n";
}

void bombino::flame::update() {
  if (levi::get_time() > last_time_ + frame_time) {
    ++cur_frame_;
    if (cur_frame_ >= 4) {
      this->delete_later();
      return;
    }
    src_rect_.x += src_rect_.width;
  }
}
