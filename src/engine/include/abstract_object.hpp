// abstract_object.hpp

#pragma once

#include <cstdint>
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

private:
  std::string texture_id_;
  bool wait_delete_;

public:
  rect src_rect_;

private:
  rect dst_rect_;
  float angle_;
  flip flip_;
};
}; // namespace levi
