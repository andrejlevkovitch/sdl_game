// tile_loader.hpp

#pragma once

#include <string>

class TiXmlElement;

namespace bombino {
class tile_loader {
public:
  void parse_tile_map(const std::string &file_name);
  /**\return id of loaded texture, if texture not loaded return empty string*/
  std::string read_tileset(const TiXmlElement *tileset);
  void read_layer(const TiXmlElement *layer);

public:
  size_t map_width_;
  size_t map_height_;
  size_t tile_width_;
  size_t tile_height_;
};
} // namespace bombino
