// main.cpp

#include <cstdlib>
#include <iostream>
#include <memory>

#include "engine.hpp"
#include "input_handler.hpp"
#include "state_machine.hpp"
#include "time.hpp"

#include "menu_state.hpp"

const int fixedFPS{20};

int main(int argc, char *argv[]) {
  auto &engine = levi::engine::instance();
  auto &input_handler = levi::input_handler::instance();

  auto menu = std::make_shared<deep_space::menu_state>();
  engine.state_machine().push_state(menu);

  bool game_stop = false;

  while (!game_stop) {
    auto last_tick = levi::get_time();

    input_handler.update();
    game_stop = input_handler.is_quit();
    for (auto &i : input_handler.get_event_list()) {
      std::cerr << i << std::endl;
    }
    engine.update();
    engine.render();

    auto dif_time = levi::get_time() - last_tick;
    if (dif_time < 1000 / fixedFPS) {
      levi::delay(1000 / fixedFPS - dif_time);
    }
  }

  return EXIT_SUCCESS;
}
