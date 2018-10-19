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
   * \return capacity of load textures
   * \warning if texture id alrady is in manager, this texture will not be
   * loaded again*/
  size_t parse_textures(const std::string &texture_file);
  /**\exception if texture_id not exist throw std::except*/
  texture get_texture(const std::string &texture_id) const;
  /**\brief get all not loaded objects whith errors, after this list will be
   * cleared.
   * \warning If texture already is in manager and manager try it load again -
   * this will not be put as not_load*/
  std::list<std::string> get_not_load_textures();
  /**\return true if texture was be created, and false other times*/
  bool create_texture(const std::string &id, const std::string &file);

private:
  std::map<std::string, texture> texture_map_;
  std::list<std::string> not_loaded_;
};
}; // namespace levi
