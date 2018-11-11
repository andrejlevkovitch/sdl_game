// main.cpp

#include "engine.hpp"
#include "input_handler.hpp"
#include "menu_config.hpp"
#include "menu_state.hpp"
#include "player.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "time.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>

#include "game_state_machine.hpp"

int main(int argc, char *argv[]) {
  auto &engine = levi::engine::instance();
  engine.texture_manager().parse_textures(levi::way_to_menu_textures +
                                          "menus_textures.xml");
  auto not_loaded = engine.texture_manager().get_not_load_textures();
  if (!not_loaded.empty()) {
    std::cerr << "problem, not all menu_textures loaded\n";
    for (auto &i : not_loaded) {
      std::cerr << i << std::endl;
    }
  }

  // have to be loaded before, then set menu!
  auto &player = levi::player::instance();
  if (player.parse_file(levi::way_to_menu_textures + "sounds.xml") == 0) {
    std::cerr << "problem, no one sound loaded\n";
  }

  auto &input_handler = levi::input_handler::instance();

  auto menu = std::make_shared<bombino::menu_state>();
  engine.state_machine().push_state(menu);

  bool game_stop = false;

  unsigned int last_tick_update{levi::get_time()};
  unsigned int last_tick_render{levi::get_time()};
  while (!game_stop) {
    engine.update(levi::get_time() - last_tick_update);
    last_tick_update = levi::get_time();
    game_stop = input_handler.is_quit();
    engine.render(levi::get_time() - last_tick_render);
    last_tick_render = levi::get_time();
  }

  return EXIT_SUCCESS;
}
