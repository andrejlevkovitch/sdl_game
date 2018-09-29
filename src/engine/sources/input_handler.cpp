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
      {::SDL_SCANCODE_UP, bind{button_code::up}()},
      {::SDL_SCANCODE_W, bind{button_code::up}()},
      {::SDL_SCANCODE_DOWN, bind{button_code::down}()},
      {::SDL_SCANCODE_S, bind{button_code::down}()},
      {::SDL_SCANCODE_RIGHT, bind{button_code::right}()},
      {::SDL_SCANCODE_D, bind{button_code::right}()},
      {::SDL_SCANCODE_LEFT, bind{button_code::left}()},
      {::SDL_SCANCODE_A, bind{button_code::left}()}};

  static std::map<::SDL_Scancode, uint8_t> last_states_keys{};

  auto time = ::SDL_GetTicks() / 100;
  if (time == last_time_) {
    return;
  }

  ::SDL_PumpEvents();

  ::SDL_Event sdl_event;
  levi::event event{};
  auto keys = ::SDL_GetKeyboardState(nullptr);
  while (::SDL_PollEvent(&sdl_event)) {
    if (sdl_event.type == SDL_QUIT) {
      event.quit = quit_event{};
      event_list_.push_back(event);
    }
  }
  for (auto &i : key_event_map) {
    if (keys[i.first]) {
      event = i.second;
      if (event.type == event_type::button_event) {
        event.button.state = button_state::pressed;
        event_list_.push_back(event);
      } else {
        event_list_.push_back(event);
      }
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

levi::input_handler::input_handler() : last_time_{} {}
