// tile.cpp

#include "tile.hpp"

bombino::tile::tile(const std::string &texture_id, levi::size size,
                    levi::vector2d pos, object_type type, unsigned frame)
    : levi::abstract_object{texture_id, size, pos}, type_{type} {
  src_rect_.x += src_rect_.width * frame;
}

void bombino::tile::update() { return; }

levi::object_type bombino::tile::type() const {
  return static_cast<levi::object_type>(type_);
}

void bombino::tile::destroy() {
  if (type_ == object_type::soft_block) {
    src_rect_.x = 0;
    src_rect_.y = 0;
    type_ = object_type::void_block;
  }
}
