// callback_map.cpp

#include "callback_map.hpp"
#include "engine.hpp"
#include "input_handler.hpp"
#include "menu_state.hpp"
#include "over_state.hpp"
#include "pause_state.hpp"
#include "player.hpp"
#include "playing_state.hpp"
#include "scene.hpp"
#include "state_machine.hpp"

bombino::callback_map &bombino::callback_map::instance() {
  static callback_map retval{};
  return retval;
}

bombino::callback_map::callback_map() {
  auto callback_play = []() {
    auto cur_scene = levi::engine::instance().state_machine().current_state();
    cur_scene->set_visibility(false);
    cur_scene->set_updatebility(false);

    auto new_scene = std::make_shared<playing_state>();
    levi::engine::instance().state_machine().push_state(new_scene);

    levi::player::instance().play("fon", false);
  };

  auto callback_exit = []() {
    levi::event event{};
    event.type = levi::event_type::quit_event;
    levi::input_handler::instance().add_event(event);
  };

  auto callback_resume = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        true);

    levi::event event{};

    event.button = levi::button_event{levi::button_code::left,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::right,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button =
        levi::button_event{levi::button_code::up, levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::down,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::select,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);

    event.button = levi::button_event{levi::button_code::left_dop,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::right_dop,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::up_dop,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::down_dop,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
    event.button = levi::button_event{levi::button_code::select_dop,
                                      levi::button_state::released};
    levi::input_handler::instance().add_event(event);
  };

  auto callback_to_menu = []() {
    while (levi::engine::instance().state_machine().current_state()->get_id() !=
           levi::id_state::menu) {
      levi::engine::instance().state_machine().pop_state();
    }
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        true);
    levi::engine::instance().state_machine().current_state()->set_visibility(
        true);
    levi::player::instance().play("fon", true);
  };

  auto callback_restart = []() {
    levi::engine::instance().state_machine().pop_state();
    levi::engine::instance().state_machine().current_state()->reload();
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        true);
    levi::engine::instance().state_machine().current_state()->set_visibility(
        true);
  };

  auto callback_to_game_over = []() {
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        false);
    levi::engine::instance().state_machine().push_state(
        std::make_shared<bombino::over_state>());
    levi::player::instance().play("fon", true);
  };

  auto callback_to_pause = []() {
    levi::engine::instance().state_machine().current_state()->set_updatebility(
        false);
    levi::engine::instance().state_machine().push_state(
        std::make_shared<pause_state>());
  };

  (*this)["play"] = callback_play;
  (*this)["exit"] = callback_exit;
  (*this)["resume"] = callback_resume;
  (*this)["to_menu"] = callback_to_menu;
  (*this)["restart"] = callback_restart;
  (*this)["to_game_over"] = callback_to_game_over;
  (*this)["to_pause"] = callback_to_pause;
}
