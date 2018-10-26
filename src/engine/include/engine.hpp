// engine.hpp

#pragma once

#include "dop_classes.hpp"
#include "event.hpp"
#include "rect.hpp"
#include "size.hpp"
#include <cstdint>

namespace levi {
class state_machine;
class texture_manager;
class abstract_object;

class engine;
class texture;

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
  /**\param angle in degrees*/
  void draw(const texture &texture, const rect &src_rect, const rect &dst_rect,
            double angle, flip flip);

private:
  engine();
  engine(const engine &) = delete;
  engine &operator=(const engine &) = delete;

private:
  void *window_;
  void *gl_context_;
  class state_machine *state_machine_;
  class texture_manager *texture_manager_;
  uint32_t shader_program_;
};

void render(engine &engine, state_machine *s_m);
}; // namespace levi
