// engine.hpp

#pragma once

#include "enums.hpp"
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
  void update(unsigned int delta_t_ms);
  void render(unsigned int delta_t_ms);
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
  unsigned int fps_;
  unsigned int ups_;
  unsigned int update_interval_;
  unsigned int render_interval_;
};
}; // namespace levi
