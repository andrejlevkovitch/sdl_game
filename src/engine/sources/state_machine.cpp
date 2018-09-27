// state_machine.cpp

#include "state_machine.hpp"
#include "scene.hpp"

#include <iostream>

levi::state_machine::state_machine() { current_state_ = states_list_.begin(); }

levi::state_machine::~state_machine(){};

void levi::state_machine::push_state(std::shared_ptr<scene> state) {
  states_list_.push_front(state);
  current_state_ = states_list_.begin();
}

void levi::state_machine::pop_state() {
  if (current_state_ != states_list_.end()) {
    ++current_state_;
  }
}

void levi::state_machine::update() {
  while (current_state_ != states_list_.begin()) {
    states_list_.pop_front();
  }
  if (!states_list_.empty()) {
    states_list_.front()->update();
  }
}

void levi::render(::SDL_Renderer *renderer, state_machine *s_m) {
  if (!s_m->states_list_.empty()) {
    render(renderer, s_m->states_list_.front().get());
  }
}
