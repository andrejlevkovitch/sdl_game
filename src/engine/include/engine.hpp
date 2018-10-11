// engine.hpp

#pragma once

#include <cstdint>

#include "event.hpp"
#include "size.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace levi {
/**\brief engine, singleton*/
class engine {
public:
  /**\brief use this for get instance of engine
   * \exception throw runtime_error if engine can't be created*/
  static engine &instance();
  ~engine();
  void update();
  void render();
  /**\return size of the window in pixels*/
  levi::size get_window_size() const;

private:
  engine();
  engine(const engine &) = delete;
  engine &operator=(const engine &) = delete;

private:
  SDL_Window *window_;
  void *gl_context_;
  levi::size win_size_;
  uint32_t v_shader_;
  uint32_t f_shader_;
  uint32_t program_;
};
}; // namespace levi
