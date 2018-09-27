// scene.hpp

#pragma once

#include <list>
#include <memory>

struct SDL_Renderer;

namespace levi {
class abstract_object;
void draw(::SDL_Renderer *renderer, abstract_object *obj);

enum class id_state { none, menu, playing, pause, over, user_id = 100 };

using item_list = std::list<std::shared_ptr<abstract_object>>;
class scene {
public:
  scene();
  virtual ~scene();
  void add_item(std::shared_ptr<abstract_object> obj);
  const levi::item_list &get_item_list();
  virtual void update();
  virtual id_state get_id() const;
  friend void render(::SDL_Renderer *renderer, scene *scene);

protected:
  levi::item_list item_list_;
};
}; // namespace levi
