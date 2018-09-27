// input_handler.hpp

#pragma once

#include <list>

#include "event.hpp"

namespace levi {
/**\brief event handler, singlton*/
class input_handler {
public:
  static input_handler &instance();
  bool is_quit() const;
  const std::list<event> &get_event_list() const;
  void read_input();
  void update();
  /**\brief add event, which will be avaliable at next iteration of game
   * cickle*/
  void add_event(levi::event event);

private:
  input_handler();
  input_handler(const input_handler &) = delete;
  input_handler &operator=(const input_handler &) = delete;

private:
  std::list<event> event_list_;
  std::list<event> next_iter_events_;
  size_t last_time_;
};
}; // namespace levi
