// bomb.hpp

#pragma once

#include <cstdint>
#include <list>

#include "abstract_object.hpp"

namespace levi {
class scene;
};

namespace bombino {
class gamer;

class bomb : public levi::abstract_object {
public:
  bomb(const std::string &texture_id, levi::size size, levi::vector2d pos,
       uint8_t power);
  ~bomb();
  void update() override;
  void blow_up();
  levi::object_type type() const override;
  void collision_handler() override;
  void set_scene(levi::scene *scene);
  /**\brief this function tell can or not gamer walk fru the bomb*/
  bool can_walk(gamer *gamer);

private:
  levi::scene *scene_;
  std::list<gamer *> gamers_hwo_can_walk_;
  uint32_t creating_time_;
  uint8_t power_;
};
}; // namespace bombino
