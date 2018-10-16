// texture_manager.hpp

#pragma once

#include <list>
#include <map>
#include <string>

#include "texture.hpp"

namespace levi {
class engine;

class texture_manager {
public:
  texture_manager();
  ~texture_manager();
  /**\brief parse xml file and create textures from datas
   * \param texture_file xml file with way to it
   * \return capacity of load textures*/
  size_t parse_textures(const std::string &texture_file);
  /**\exception if texture_id not exist throw std::except*/
  texture get_texture(const std::string &texture_id) const;
  /**\brief get all not loaded objects whith errors, after this list will be
   * cleared*/
  std::list<std::string> get_not_load_objects();

private:
  std::map<std::string, texture> texture_map_;
  std::list<std::string> not_loaded_;
};
}; // namespace levi
