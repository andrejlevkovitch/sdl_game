// state_machine.cpp

#include "state_machine.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include <algorithm>

levi::state_machine::state_machine() { current_state_ = states_list_.begin(); }

levi::state_machine::~state_machine(){};

void levi::state_machine::push_state(std::shared_ptr<scene> state) {
  current_state_ = states_list_.insert(current_state_, state);
}

void levi::state_machine::pop_state() {
  if (current_state_ != states_list_.end()) {
    ++current_state_;
  }
}

void levi::state_machine::update() {
  if (current_state_ != states_list_.begin()) {
    states_list_.erase(states_list_.begin(), current_state_);
  }
  std::for_each(states_list_.rbegin(), states_list_.rend(),
                [](std::shared_ptr<scene> item) {
                  if (item->is_updateble()) {
                    item->update();
                  }
                });
}

std::shared_ptr<levi::scene> levi::state_machine::current_state() {
  if (current_state_ == states_list_.end()) {
    throw std::runtime_error{"state is absent"};
  }
  return *current_state_;
}

const std::list<std::shared_ptr<levi::scene>> &
levi::state_machine::get_states_list() const {
  return this->states_list_;
}

void levi::render(engine &engine, const state_machine &state_machine) {
  auto &states = state_machine.get_states_list();
  std::for_each(states.rbegin(), states.rend(), [&](const auto &item) {
    if (item->is_visible()) {
      render(engine, *item);
    }
  });
}
