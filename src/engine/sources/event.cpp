// event.cpp

#include "event.hpp"
#include <SDL2/SDL.h>

std::ostream &levi::operator<<(std::ostream &out, const levi::event &event) {
  switch (event.type) {
  case event_type::quit_event:
    out << "quit";
    break;
  case event_type::button_event:
    switch (event.button.code) {
    case levi::button_code::select:
      out << "select ";
      break;
    case levi::button_code::up:
      out << "up ";
      break;
    case levi::button_code::right:
      out << "right ";
      break;
    case levi::button_code::left:
      out << "left ";
      break;
    case levi::button_code::down:
      out << "down ";
      break;
    default:
      out << "unknow button ";
      break;
    }
    out << ((event.button.state == button_state::pressed) ? "pressed"
                                                          : "released");
    break;
  case event_type::pause_event:
    out << "pause";
    break;
  default:
    out << "unknow event";
    break;
  }

  return out;
}
