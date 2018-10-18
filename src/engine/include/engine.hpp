// engine.hpp

#pragma once

#include "dop_clases.hpp"
#include "event.hpp"
#include "rect.hpp"
#include "size.hpp"

struct SDL_Window;

namespace levi {
class state_machine;
class texture_manager;
class abstract_object;

class engine;
class texture;
void render(engine &engine, state_machine *s_m);

/**\brief engine, singleton*/
class engine {
public:
  /**\brief use this for get instance of engine
   * \exception throw runtime_error if engine can't be created*/
  static engine &instance();
  ~engine();
  class state_machine &state_machine();
  class texture_manager &texture_manager();
  void update();
  void render();
  /**\return size of the window in pixels*/
  levi::size get_window_size() const;
  friend void draw(engine &engine, abstract_object *obj);
  /**\param angle in degrees*/
  void draw(const texture &texture, const rect &src_rect, const rect &dst_rect,
            double angle, flip flip);

private:
  engine();
  engine(const engine &) = delete;
  engine &operator=(const engine &) = delete;

private:
  SDL_Window *window_;
  void *gl_context_;
  class state_machine *state_machine_;
  class texture_manager *texture_manager_;
  uint32_t shader_program_;
  uint32_t vao_;
  uint32_t vbo_;
  uint32_t ebo_;
  int32_t pos_;
  int32_t tex_pos_;
};
}; // namespace levi
