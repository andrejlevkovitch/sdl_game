// abstract_object.hpp

#pragma once

#include "enums.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "vector2d.hpp"
#include <cstdint>
#include <list>
#include <string>
#include <tuple>

namespace levi {
class engine;
class scene;
class abstract_object;
void draw(engine &engine, const abstract_object &obj);

/**\brief abstract base class for all objects*/
class abstract_object {
public:
  /**\param texture_id id of texture which use in the object
   * \param size frame size
   * \param pos begin pos of top-left corner*/
  abstract_object(const std::string &texture_id, levi::size size, vector2d pos);
  abstract_object(const abstract_object &) = delete;
  abstract_object &operator=(const abstract_object &) = delete;
  virtual ~abstract_object();
  virtual void update() = 0;
  /**\return type of object, which have to have value more, then
   * object_type::user_type for custom user types*/
  virtual object_type type() const;
  /**\brief call this method if you wannt to delete it.
   * \warning don't use destructor instead!*/
  void delete_later();
  /**\brief return true, if for object was be called delete_later()*/
  bool is_for_delete();
  void set_size(levi::size size);
  levi::size get_size() const;
  /**\brief set position of the top-left corner
   * \warning value of position store in int values, and when you set float pos,
   * this pos will be rounded!*/
  void set_pos(vector2d pos);
  /**\return current pixel position of the top-left corner*/
  levi::vector2d get_pos() const;
  /**\breif add to current angle new value
   * \return angle past the operation*/
  virtual float rotate(float value);
  virtual void set_angle(float angle);
  /**\return current angle in degres [0, 360)*/
  float get_angle() const;
  void set_flip(flip f);
  flip get_flip() const;
  friend void draw(engine &engine, const abstract_object &obj);
  /**\return by default this function return dst_rect_, but, if you need onother
   * value, just redefine it
   * \brief this function use for find collisions.*/
  virtual rect get_rectangle() const;
  /**\brief when scene intake object, it set pointer on it in the object
   * \warning this method call only if object loaded by add_item, otherwise,
   * pointer to scene will be nullptr */
  virtual void set_scene(scene *scene);

protected:
  scene *scene_;
  std::string texture_id_;
  /**\brief this rect store rectangle of current sprite in image coordinates
   * (begin from [0, 0] in integer values)*/
  rect src_rect_;
  /**\brief this rect store rectangle of current screen position in screen
   * coordinates (begin from [0, 0] in integer values)*/
  rect dst_rect_;
  /**\brief angle in degrees*/
  float angle_;
  flip flip_;
  bool wait_delete_;
};
}; // namespace levi
