// engine.hpp

#pragma once

#include "event.hpp"
#include "size.hpp"

struct SDL_Window;
struct SDL_Renderer;

namespace levi {
class state_machine;
class scene;
void render(::SDL_Renderer *renderer, state_machine *s_m);

/**\brief engine, singleton*/
class engine {
public:
  /**\brief use this for get instance of engine
   * \exception throw runtime_error if engine can't be created*/
  static engine &instance();
  ~engine();
  class state_machine &state_machine();
  void update();
  void render() const;
  /**\return size of the window in pixels*/
  levi::size get_window_size() const;

private:
  engine();
  engine(const engine &) = delete;
  engine &operator=(const engine &) = delete;

private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  class state_machine *state_machine_;
  levi::size win_size_;
};
}; // namespace levi
