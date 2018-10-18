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

#include "base64.h"
#include <vector>
#include <zlib.h>

const int fixedFPS{20};

int main(int argc, char *argv[]) {

  std::string code{"eJytk1sKACAIBH3c/879JCzL2kcaDEVOIlZpZtmAo3MKB+KivIqhjzniA2/"
                   "yK++1p+rkGK+VY8JRZzoHe+6CoJnhHJM6NvqxdS8b72PjnU7/ywExDQHW"};
  auto decode = base64_decode(code);

  size_t size = 17 * 13 * sizeof(int);
  std::vector<int> vec(17 * 13, 0);
  if (::uncompress(reinterpret_cast<Bytef *>(&vec[0]), &size,
                   reinterpret_cast<Bytef *>(&decode[0]),
                   decode.size()) != Z_OK) {
    std::cerr << "problem with uncompress\n" << std::endl;
    return EXIT_FAILURE;
  }

  for (int i{}; i < 13; ++i) {
    for (int j{}; j < 17; ++j) {
      std::cerr << vec[i * 17 + j] << ' ';
    }
    std::cerr << std::endl;
  }
  std::cerr << std::endl;

  auto &engine = levi::engine::instance();
  engine.texture_manager().parse_textures(bombino::way_to_files +
                                          "menus_textures.xml");
  engine.texture_manager().parse_textures(bombino::way_to_files +
                                          "bombino_textures.xml");
  auto not_loaded = engine.texture_manager().get_not_load_objects();
  if (!not_loaded.empty()) {
    for (auto &i : not_loaded) {
      std::cerr << i << std::endl;
    }
  }

  auto &input_handler = levi::input_handler::instance();

  auto menu = std::make_shared<bombino::menu_state>();
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
