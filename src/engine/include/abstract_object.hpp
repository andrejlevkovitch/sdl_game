// abstract_object.hpp

#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <tuple>

#include "dop_clases.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "vector2d.hpp"

namespace levi {
class engine;
enum object_type { unknown, user_type = 100 };

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
  void set_pos(vector2d pos);
  levi::vector2d get_pos() const;
  /**\breif add to current angle new value
   * \return angle past the operation*/
  virtual float rotate(float value);
  virtual void set_angle(float angle);
  /**\return current angle in diapason [0, 360)*/
  float get_angle() const;
  void set_flip(flip f);
  flip get_flip() const;
  friend void draw(engine &engine, abstract_object *obj);
  /**\return by default this function return dst_rect_, but, if you need onother
   * value, just redefine it
   * \brief this function use for find collisions.*/
  virtual rect get_rectangle() const;
  /**\return value of need_collisions_flag_.
   * \brief this function use scene for collision-finding*/
  bool need_collisions() const;
  /**\brief you need redefine this function, if you want handle collisions in
   * fly. If any collision detected, then scene call this method*/
  virtual void collision_handler();

public:
  std::string texture_id_;
  bool wait_delete_;

protected:
  rect src_rect_;

private:
  rect dst_rect_;
  float angle_;
  flip flip_;

protected:
  bool need_collisions_flag_;

public:
  std::list<abstract_object *> collisions_;
};
}; // namespace levi
