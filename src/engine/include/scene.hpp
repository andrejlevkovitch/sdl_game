// scene.hpp

#pragma once

#include "enums.hpp"
#include "rect.hpp"
#include <list>
#include <memory>

namespace levi {
class engine;
class abstract_object;

using item_list = std::list<std::shared_ptr<abstract_object>>;

class scene {
public:
  scene();
  virtual ~scene();
  /**\warning if you want to overload this method, you must remember, that
   * method have to call set_scene for every added object, otherwise in objects
   * you will have nullptr in scene_*/
  virtual void add_item(std::shared_ptr<abstract_object> obj);
  virtual void add_light(std::shared_ptr<abstract_object> obj);
  const levi::item_list &get_item_list() const;
  const levi::item_list &get_light_list() const;
  virtual void update();
  /**\return all coolisions for input rectengle*/
  virtual std::list<abstract_object *> get_collisions_for(rect rect);
  /**\brief if you want to set new id, which not have in enum, then set value
   * after levi::id_state::user_id, becose all values before is rezerved*/
  virtual id_state get_id() const;
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
  virtual void render(engine &engine) const;
  virtual void calculate_light(engine &engine) const;

protected:
  levi::item_list item_list_;
  levi::item_list light_list_;

private:
  bool is_visible_;
  bool is_updateble_;
};

}; // namespace levi
