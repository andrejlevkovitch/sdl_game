// scene.hpp

#pragma once

#include <list>
#include <memory>

struct SDL_Renderer;

namespace levi {
class engine;
class abstract_object;
void draw(engine &engine, abstract_object *obj);

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
  friend void render(engine &engine, scene *scene);
  /**\brief hides or shows scene
   * \param value false if you want hide, true if show. By default scene is
   * visible*/
  void set_visibility(bool value);
  /**\return true if scene is visible, and false if not*/
  bool is_visible();
  /**\brief use this method if you need disable updating scene
   * \param value false if scene can't be updated, true if show. By default
   * scene is updateble*/
  void set_updatebility(bool value);
  /**\return true if scene can be update, and false if not*/
  bool is_updateble();

protected:
  levi::item_list item_list_;

private:
  bool is_visible_;
  bool is_updateble_;
};
}; // namespace levi