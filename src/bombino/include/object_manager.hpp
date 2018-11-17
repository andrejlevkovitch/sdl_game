// object_manager.hpp

#pragma once

#include "size.hpp"
#include <cstdint>
#include <map>

namespace bombino {
struct object_params {
  object_params();
  /**\param texture_width it needed if your sprites placed in 2D, if your sprite
   * plased in one row, then you can set 0*/
  object_params(levi::size object_size, const std::string &texture_id,
                unsigned texture_width, uint8_t front_frame, uint8_t back_frame,
                uint8_t side_frame, uint8_t frame_count);

  std::string texture_id;
  levi::size object_size;
  unsigned texture_width;
  uint8_t front_frame;
  uint8_t back_frame;
  uint8_t side_frame;
  uint8_t frame_count;
};

/**\brief first param - objects alias, second - objects params*/
using object_map = std::map<std::string, bombino::object_params>;

// TODO I have to improve parser
class object_manager {
public:
  static object_manager &instance();
  /**\except if couldn't load file or couldn't find root
   * \return count of loaded objects. If type or texture_id couldn't be finded,
   * object will not load
   * \brief parse file and load objects params in manager. Manager already have
   * some type, this will be loaded again*/
  unsigned parse_file(const std::string &file_name);
  /**\except if type not founded, throw std::out_of_range*/
  const object_params &get_obj_params(const std::string &alias);

private:
  object_manager();
  object_manager(const object_manager &) = delete;
  object_manager &operator=(const object_manager &) = delete;

private:
  object_map obj_map_;
};
}; // namespace bombino
