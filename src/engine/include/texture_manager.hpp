// texture_manager.hpp

#pragma once

#include <map>
#include <string>

struct SDL_Texture;

namespace levi {
class engine;

class texture_manager {
public:
  texture_manager(engine &engine);
  ~texture_manager();
  /**\brief parse xml file and create textures from datas
   * \param texture_file xml file with way to it
   * \return capacity of load textures*/
  size_t parse_textures(const std::string &texture_file);
  SDL_Texture *get_texture(const std::string &texture_id);

private:
  engine &engine_;
  std::map<std::string, SDL_Texture *> texture_map_;
};
}; // namespace levi
