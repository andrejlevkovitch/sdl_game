// engine.cpp

#include "engine.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

#include "event.hpp"
#include "state_machine.hpp"

levi::engine &levi::engine::instance() {
  static engine retval{};
  return retval;
}

levi::engine::engine()
    : window_{nullptr}, renderer_{nullptr}, state_machine_{
                                                new levi::state_machine} {
  if (::SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  window_ =
      ::SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 640, 480, ::SDL_WINDOW_SHOWN);
  if (!window_) {
    throw std::runtime_error{::SDL_GetError()};
  }

  renderer_ = ::SDL_CreateRenderer(window_, -1, 0);
  if (!renderer_) {
    throw std::runtime_error{::SDL_GetError()};
  }

  if (::SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  int w{};
  int h{};
  ::SDL_GetRendererOutputSize(renderer_, &w, &h);
  win_size_ = size{w, h};
}

levi::engine::~engine() {
  ::SDL_DestroyWindow(window_);
  ::SDL_DestroyRenderer(renderer_);
  delete state_machine_;
  ::SDL_Quit();
}

levi::state_machine &levi::engine::state_machine() { return *state_machine_; }

void levi::engine::update() { state_machine_->update(); }

void levi::engine::render() const {
  ::SDL_RenderClear(renderer_);
  levi::render(renderer_, state_machine_);
  ::SDL_RenderPresent(renderer_);
}

levi::size levi::engine::get_window_size() const { return win_size_; }
