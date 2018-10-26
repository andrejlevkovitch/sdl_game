// bomb.cpp

#include "bomb.hpp"

#include <algorithm>

#include "flame.hpp"
#include "gamer.hpp"
#include "object_manager.hpp"
#include "object_types.hpp"
#include "player.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "time.hpp"

#include <iostream>

const unsigned time_to_explosion{4000};

namespace bombino {
inline void explosion(levi::scene *scene, levi::abstract_object *obj) {
  switch (static_cast<object_type>(obj->type())) {
  case object_type::gamer1:
  case object_type::gamer2:
    dynamic_cast<gamer *>(obj)->kill();
    break;
  case object_type::power:
    obj->delete_later();
    break;
  case object_type::soft_block:
  case object_type::void_block: {
    dynamic_cast<tile *>(obj)->destroy();
    auto &flame_params = object_manager::instance().get_obj_params("flame");
    levi::vector2d flame_pos = obj->get_pos();
    flame_pos.x += (obj->get_size().width - flame_params.object_size.width) / 2;
    flame_pos.y +=
        (obj->get_size().height - flame_params.object_size.height) / 2;
    scene->add_item(std::make_shared<class flame>(
        flame_params.texture_id, flame_params.object_size, flame_pos,
        flame_params.frame_count));
  } break;
  case object_type::bomb:
    // here we check: this bomb alrady blow up, or not
    if (!obj->is_for_delete()) {
      dynamic_cast<class bomb *>(obj)->blow_up();
    }
    break;
  default:
    break;
  }
} // namespace bombino

/**\return false if meet on the way any solid or soft block
 * \brif in this function call func explosion for every collision*/
inline bool calculate_for_rect(levi::scene *scene, levi::rect rect) {
  bool retval{true};
  for (auto &j : scene->get_collisions_for(rect)) {
    if (j->type() == static_cast<levi::object_type>(object_type::soft_block) ||
        j->type() == static_cast<levi::object_type>(object_type::solid_block)) {
      retval = false;
    }
    explosion(scene, j);
  }
  return retval;
}
} // namespace bombino

bombino::bomb::bomb(const std::string &texture_id, levi::size size,
                    levi::vector2d pos, uint8_t power)
    : levi::abstract_object{texture_id, size, pos},
      creating_time_{levi::get_time()}, power_{power} {
  need_collisions_flag_ = true;
}

bombino::bomb::~bomb() {}

void bombino::bomb::update() {
  if (levi::get_time() > creating_time_ + time_to_explosion) {
    blow_up();
  }
}

void bombino::bomb::blow_up() {
  // this have to be first, becose we check by it repides calls
  this->delete_later();

  levi::player::instance().play("explosion", true);

  auto tile_size =
      object_manager::instance().get_obj_params("tile").object_size;

  // here we destroy all in tile, there set bomb
  calculate_for_rect(scene_, dst_rect_);

  bool side1{true};
  bool side2{true};
  for (int i{1}; i < power_ + 1; ++i) {
    if (side1) {
      auto temp_rect = dst_rect_;
      temp_rect.x -= i * tile_size.width;

      side1 = calculate_for_rect(scene_, temp_rect);
    }
    if (side2) {
      auto temp_rect = dst_rect_;
      temp_rect.x += i * tile_size.width;

      side2 = calculate_for_rect(scene_, temp_rect);
    }
  }
  side1 = side2 = true;
  for (int i{1}; i < power_ + 1; ++i) {
    if (side1) {
      auto temp_rect = dst_rect_;
      temp_rect.y -= i * tile_size.height;

      side1 = calculate_for_rect(scene_, temp_rect);
    }
    if (side2) {
      auto temp_rect = dst_rect_;
      temp_rect.y += i * tile_size.height;

      side2 = calculate_for_rect(scene_, temp_rect);
    }
  }
}

levi::object_type bombino::bomb::type() const {
  return static_cast<levi::object_type>(bombino::object_type::bomb);
}

void bombino::bomb::collision_handler() {
  if (!gamers_hwo_can_walk_.empty()) {
    std::list<gamer *> cur_gamers;
    for (auto &i : collisions_) {
      if (i->type() == static_cast<levi::object_type>(object_type::gamer1) ||
          i->type() == static_cast<levi::object_type>(object_type::gamer2)) {
        cur_gamers.push_back(reinterpret_cast<gamer *>(i));
      }
    }
    for (auto i = gamers_hwo_can_walk_.begin();
         i != gamers_hwo_can_walk_.end();) {
      if (std::find(cur_gamers.begin(), cur_gamers.end(), *i) ==
          cur_gamers.end()) {
        i = gamers_hwo_can_walk_.erase(i);
      } else {
        ++i;
      }
    }
  }
}

void bombino::bomb::set_scene(levi::scene *scene) {
  scene_ = scene;
  for (auto &i : scene_->get_collisions_for(this->get_rectangle())) {
    if (i->type() == static_cast<levi::object_type>(object_type::gamer1) ||
        i->type() == static_cast<levi::object_type>(object_type::gamer2)) {
      gamers_hwo_can_walk_.push_back(reinterpret_cast<gamer *>(i));
    }
  }
}

bool bombino::bomb::can_walk(gamer *gamer) {
  for (auto &i : gamers_hwo_can_walk_) {
    if (gamer == i) {
      return true;
    }
  }
  return false;
}
