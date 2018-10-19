// playing_state.cpp

#include "playing_state.hpp"

#include "config.hpp"
#include "engine.hpp"
#include "gamer.hpp"
#include "input_handler.hpp"
#include "object_factory.hpp"
#include "pause_state.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "tile.hpp"
#include "tile_loader.hpp"

bombino::playing_state::playing_state() {
  // load textures for objects
  levi::engine::instance().texture_manager().parse_textures(
      bombino::way_to_files + "bombino_textures.xml");
  // tile_loading
  tile_loader t_loader;
  t_loader.parse_tile_map(bombino::way_to_objects + "map0.tmx");
  levi::engine::instance().texture_manager().create_texture(
      t_loader.image_id_, bombino::way_to_objects + t_loader.image_file_name_);
  for (const auto &i : t_loader.get_tiles()) {
    item_list_.push_back(i);
  }
  // object_loading
  parse_state(bombino::way_to_objects + "bombino_states.xml", "playing",
              item_list_);
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
}

levi::id_state bombino::playing_state::get_id() const {
  return levi::id_state::playing;
}
