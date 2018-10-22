// bomb.cpp

#include "bomb.hpp"
#include "flame.hpp"
#include "gamer.hpp"
#include "object_types.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "time.hpp"

#include <iostream>

// TODO hardkod!!!
namespace bombino {
inline void explosition(levi::scene *scene, levi::abstract_object *obj) {
  if (obj->type() == static_cast<levi::object_type>(gamer1) ||
      obj->type() == static_cast<levi::object_type>(gamer2)) {
    dynamic_cast<gamer *>(obj)->kill();
  }
  if (obj->type() == static_cast<levi::object_type>(object_type::soft_block) ||
      obj->type() == static_cast<levi::object_type>(object_type::void_block)) {
    dynamic_cast<tile *>(obj)->destroy();
    scene->add_item(std::make_shared<class flame>(
        "flame", levi::size{48, 48},
        levi::vector2d{obj->get_pos() + levi::vector2d{8, 8}}));
  }
}
} // namespace bombino

const unsigned time_to_explosition{4000};

bombino::bomb::bomb(const std::string &texture_id, levi::size size,
                    levi::vector2d pos, uint8_t power)
    : levi::abstract_object{texture_id, size, pos},
      creating_time_{levi::get_time()}, power_{power} {
  std::cerr << "BOMB\n";
}

bombino::bomb::~bomb() { std::cerr << "explosition\n"; }

void bombino::bomb::update() {
  if (levi::get_time() > creating_time_ + time_to_explosition) {
    levi::rect temp_rect{dst_rect_};
    temp_rect.x -= 64;
    for (int i{}; i < power_ * 2 + 1; ++i) {
      for (auto &i : scene->get_collisions_for(temp_rect)) {
        explosition(scene, i);
      }
      temp_rect.x += 64;
    }
    temp_rect = dst_rect_;
    temp_rect.y -= 64;
    for (int i{}; i < power_ * 2 + 1; ++i) {
      for (auto &i : scene->get_collisions_for(temp_rect)) {
        explosition(scene, i);
      }
      temp_rect.y += 64;
    }

    this->delete_later();
  }
}

levi::object_type bombino::bomb::type() const {
  return static_cast<levi::object_type>(bombino::object_type::bomb);
}
