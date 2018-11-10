// bomb.hpp

#pragma once

#include "abstract_object.hpp"
#include <cstdint>
#include <list>

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
  void collision_handler();
  /**\brief this function tell can or not gamer walk fru the bomb*/
  bool can_walk(gamer *gamer);
  virtual void set_scene(levi::scene *scene) override;
  void kick(levi::vector2d direction, float velocity);

private:
  std::list<gamer *> gamers_hwo_can_walk_;
  uint32_t creating_time_;
  levi::vector2d direction_;
  float velocity_;
  uint8_t power_;
};
}; // namespace bombino
