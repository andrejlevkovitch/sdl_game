// event_hanler.cpp

#include "input_handler.hpp"

#include <algorithm>
#include <iterator>
#include <map>

#include <SDL2/SDL.h>

levi::input_handler &levi::input_handler::instance() {
  static input_handler retval{};
  return retval;
}

bool levi::input_handler::is_quit() const {
  for (auto &i : event_list_) {
    if (i.type == event_type::quit_event) {
      return true;
    }
  }
  return false;
}

const std::list<levi::event> &levi::input_handler::get_event_list() const {
  return event_list_;
}

namespace levi {
class bind {
public:
  explicit bind(const pause_event &q_e) : event_{} { event_.pause = q_e; };
  explicit bind(const button_code b_c) : event_{} {
    event_.button = button_event{b_c, button_state::none};
  };
  event operator()() { return event_; };

private:
  event event_;
};
} // namespace levi

void levi::input_handler::read_input() {
  static const std::map<::SDL_Scancode, levi::event> key_event_map{
      {::SDL_SCANCODE_ESCAPE, bind{pause_event{}}()},
      {::SDL_SCANCODE_RETURN, bind{button_code::select}()},
      {::SDL_SCANCODE_E, bind{button_code::select_dop}()},
      {::SDL_SCANCODE_UP, bind{button_code::up}()},
      {::SDL_SCANCODE_W, bind{button_code::up_dop}()},
      {::SDL_SCANCODE_DOWN, bind{button_code::down}()},
      {::SDL_SCANCODE_S, bind{button_code::down_dop}()},
      {::SDL_SCANCODE_RIGHT, bind{button_code::right}()},
      {::SDL_SCANCODE_D, bind{button_code::right_dop}()},
      {::SDL_SCANCODE_LEFT, bind{button_code::left}()},
      {::SDL_SCANCODE_A, bind{button_code::left_dop}()}};

  levi::event event{};
  ::SDL_Event sdl_event{};
  while (::SDL_PollEvent(&sdl_event)) {
    switch (sdl_event.type) {
    case ::SDL_QUIT:
      event.quit = quit_event{};
      event_list_.push_back(event);
      break;
    case ::SDL_KEYUP:
    case ::SDL_KEYDOWN: {
      bool coincidence = false;
      for (auto i : key_event_map) {
        if (sdl_event.key.keysym.scancode == i.first) {
          event = i.second;
          coincidence = true;
          break;
        }
      }
      if (coincidence) {
        if (event.type == event_type::button_event) {
          event.button.state = (sdl_event.key.state == SDL_PRESSED)
                                   ? button_state::pressed
                                   : button_state::released;
        }
        event_list_.push_back(event);
      }
    } break;
    default:
      break;
    }
  }
}

void levi::input_handler::update() {
  event_list_.clear();
  if (!next_iter_events_.empty()) {
    std::copy(next_iter_events_.begin(), next_iter_events_.end(),
              std::back_inserter(event_list_));
    next_iter_events_.clear();
  }
  read_input();
}

void levi::input_handler::add_event(levi::event event) {
  next_iter_events_.push_back(event);
}

levi::input_handler::input_handler() {}
