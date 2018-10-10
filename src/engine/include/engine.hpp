// engine.hpp

#pragma once

#include "event.hpp"
#include "size.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace levi {
class state_machine;
class texture_manager;
class abstract_object;

class engine;
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
  SDL_Texture *create_texture(const std::string &file_name);
  friend void draw(engine &engine, abstract_object *obj);

private:
  engine();
  engine(const engine &) = delete;
  engine &operator=(const engine &) = delete;

private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  class state_machine *state_machine_;
  class texture_manager *texture_manager_;
  levi::size win_size_;
};
}; // namespace levi
