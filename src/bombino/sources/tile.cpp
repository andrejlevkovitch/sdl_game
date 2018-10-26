// tile.cpp

#include "tile.hpp"

#include <random>

#include "object_manager.hpp"
#include "power.hpp"
#include "scene.hpp"

bombino::tile::tile(const std::string &texture_id, levi::size size,
                    levi::vector2d pos, object_type type, unsigned frame)
    : levi::abstract_object{texture_id, size, pos}, type_{type}, power_shance_{
                                                                     5} {
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

    std::random_device rd{};
    // TODO problem for windows
    std::default_random_engine re{rd()};
    std::uniform_int_distribution<int> dist{0, power_shance_ - 1};

    if (!dist(re)) {
      std::uniform_int_distribution<int> d_power{0, powers::size - 1};
      powers p = static_cast<powers>(d_power(re));
      auto power_params =
          object_manager::instance().get_obj_params(powers_names[p]);
      levi::vector2d pos = get_pos();
      pos.x += (get_size().width - power_params.object_size.width) / 2;
      pos.y += (get_size().height - power_params.object_size.height) / 2;
      scene_->add_item(std::make_shared<class power>(
          power_params.texture_id, power_params.object_size, pos, p,
          power_params.front_frame));
    }
  }
}
