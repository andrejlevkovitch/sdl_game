// event.hpp

#pragma once

#include <cstdint>
#include <ostream>

namespace levi {
enum class event_type : uint8_t { none, quit_event, button_event, pause_event };
enum class button_code : uint8_t {
  none,
  left,
  right,
  up,
  down,
  select,
  left_dop,
  right_dop,
  up_dop,
  down_dop,
  select_dop,
  show_info
};
enum class button_state : uint8_t { none, pressed, released };

class quit_event {
public:
  quit_event() : type{event_type::quit_event} {};
  event_type type;
};

class button_event {
public:
  button_event(button_code b, button_state s)
      : type{event_type::button_event}, code{b}, state{s} {};
  event_type type;
  button_code code;
  button_state state;
};

class pause_event {
public:
  pause_event() : type{event_type::pause_event} {};
  event_type type;
};

union event {
  event_type type;
  quit_event quit;
  button_event button;
  pause_event pause;
  uint8_t size[40];
};

std::ostream &operator<<(std::ostream &out, const event &event);
}; // namespace levi
