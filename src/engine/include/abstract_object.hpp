// abstract_object.hpp

#pragma once

#include <cstdint>
#include <string>
#include <tuple>

#include "size.hpp"
#include "vector2d.hpp"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

namespace levi {
enum object_type { unknown, user_type = 100 };
enum class flip { none, horizontal, vertical };

using params = std::tuple<std::string, levi::size, vector2d>;

/**\brief abstract base class for all objects*/
class abstract_object {
public:
  abstract_object(params params);
  /**\param file_name name of file with path*/
  abstract_object(const std::string &file_name);
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
  /**\biref set current sprite frame, by default 0*/
  void set_frame(int frame);
  int get_frame() const;
  /**\breif add to current angle new value
   * \return angle past the operation*/
  virtual float rotate(float value);
  virtual void set_angle(float angle);
  /**\return current angle in diapason [0, 360)*/
  float get_angle() const;
  void set_flip(flip f);
  flip get_flip() const;
  friend bool load(::SDL_Renderer *renderer, abstract_object *obj);
  friend void draw(::SDL_Renderer *renderer, abstract_object *obj);

private:
  std::string file_name_;
  bool wait_delete_;
  ::SDL_Texture *texture_;
  ::SDL_Rect *src_rect_;
  ::SDL_Rect *dst_rect_;
  unsigned frame_;
  float angle_;
  flip flip_;
};
}; // namespace levi
