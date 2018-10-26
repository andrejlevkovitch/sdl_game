// input_handler.hpp

#pragma once

#include "event.hpp"
#include <list>

namespace levi {
/**\brief event handler, singlton*/
class input_handler {
public:
  static input_handler &instance();
  bool is_quit() const;
  const std::list<event> &get_event_list() const;
  void update();
  /**\brief add event, which will be avaliable at next iteration of game
   * circle*/
  void add_event(levi::event event);

private:
  input_handler();
  input_handler(const input_handler &) = delete;
  input_handler &operator=(const input_handler &) = delete;
  /**\brief read sdl event and made from they levi::event(s)*/
  void read_input();

private:
  std::list<event> event_list_;
  /**\brief this neede for user event addition*/
  std::list<event> next_iter_events_;
};
}; // namespace levi
