// main.cpp

#include <cstdlib>
#include <iostream>
#include <memory>

#include "config.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "menu_state.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "time.hpp"

const int fixedFPS{20};

int main(int argc, char *argv[]) {
  auto &engine = levi::engine::instance();
  engine.texture_manager().parse_textures(deep_space::way_to_files +
                                          "textures.xml");
  auto not_loaded = engine.texture_manager().get_not_load_objects();
  if (!not_loaded.empty()) {
    for (auto &i : not_loaded) {
      std::cerr << i << std::endl;
    }
  }

  auto &input_handler = levi::input_handler::instance();

  auto menu = std::make_shared<deep_space::menu_state>();
  engine.state_machine().push_state(menu);

  bool game_stop = false;

  while (!game_stop) {
    auto last_tick = levi::get_time();

    input_handler.update();
    game_stop = input_handler.is_quit();
    //  for (auto &i : input_handler.get_event_list()) {
    //    std::cerr << i << std::endl;
    //  }
    engine.update();
    engine.render();

    auto dif_time = levi::get_time() - last_tick;
    if (dif_time < 1000 / fixedFPS) {
      levi::delay(1000 / fixedFPS - dif_time);
    }
  }

  return EXIT_SUCCESS;
}
