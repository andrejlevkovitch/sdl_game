// tile.cpp

#include "tile.hpp"

bombino::tile::tile(const std::string &texture_id, levi::size size,
                    levi::vector2d pos)
    : levi::abstract_object{texture_id, size, pos} {}

void bombino::tile::update() { return; }

levi::object_type bombino::tile::type() const {
  return levi::object_type::unknown;
}

void bombino::tile::destroy() {}
