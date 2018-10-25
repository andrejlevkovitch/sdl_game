// playing_state.cpp

#include "playing_state.hpp"

#include "bomb.hpp"
#include "engine.hpp"
#include "gamer.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "objects_config.hpp"
#include "pause_state.hpp"
#include "power.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "tile.hpp"
#include "tile_loader.hpp"

bombino::playing_state::playing_state() {
  // load textures for objects
  levi::engine::instance().texture_manager().parse_textures(
      bombino::way_to_objects + "bombino_textures.xml");
  // tile_loading
  tile_loader t_loader;
  t_loader.parse_tile_map(bombino::way_to_objects + "map0.tmx");
  levi::engine::instance().texture_manager().create_texture(
      t_loader.image_id_, bombino::way_to_objects + t_loader.image_file_name_);
  for (const auto &i : t_loader.get_tiles()) {
    add_item(i);
  }
  // object_loading
  auto new_items =
      parse_state(bombino::way_to_objects + "bombino_states.xml", "playing");
  for (auto &i : new_items) {
    if (i->type() ==
            static_cast<levi::object_type>(bombino::object_type::gamer1) ||
        i->type() ==
            static_cast<levi::object_type>(bombino::object_type::gamer2)) {
      dynamic_cast<bombino::gamer *>(i.get())->scene = this;
    }
    item_list_.push_back(i);
  }
}

void bombino::playing_state::add_item(
    std::shared_ptr<levi::abstract_object> obj) {
  if (obj->type() ==
          static_cast<levi::object_type>(bombino::object_type::gamer1) ||
      obj->type() ==
          static_cast<levi::object_type>(bombino::object_type::gamer2)) {
    dynamic_cast<bombino::gamer *>(obj.get())->scene = this;
  }
  if (obj->type() ==
      static_cast<levi::object_type>(bombino::object_type::bomb)) {
    dynamic_cast<class bomb *>(obj.get())->set_scene(this);
  }
  if (obj->type() == static_cast<levi::object_type>(object_type::soft_block)) {
    dynamic_cast<class tile *>(obj.get())->scene = this;
  }
  levi::scene::add_item(obj);
}

void bombino::playing_state::update() {
  auto events = levi::input_handler::instance().get_event_list();
  for (auto &i : events) {
    if (i.type == levi::event_type::pause_event) {
      levi::engine::instance()
          .state_machine()
          .current_state()
          ->set_updatebility(false);
      levi::engine::instance().state_machine().push_state(
          std::make_shared<pause_state>());
      return;
    }
  }

  levi::scene::update();
  levi::scene::collision_detecting();
}

levi::id_state bombino::playing_state::get_id() const {
  return levi::id_state::playing;
}
