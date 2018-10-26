// tile_loader.hpp

#pragma once

#include "size.hpp"
#include <list>
#include <memory>
#include <string>
#include <vector>

class TiXmlElement;

namespace bombino {
class tile;

/**\brief this class load special .tmx files created by Tiled program
 * \TODO nowadays it work only if map created by one png file, with zero
 * margin*/
class tile_loader {
public:
  /**\except throw std::exception, read output for more information*/
  void parse_tile_map(const std::string &file_name);
  const std::list<std::shared_ptr<tile>> get_tiles() const;

private:
  void read_tileset(const TiXmlElement *tileset);
  void read_layer(const TiXmlElement *layer);
  void create_tiles();

private:
  levi::size map_size_;
  levi::size tile_size_;
  std::vector<int> layout_;

public:
  std::string image_id_;
  std::string image_file_name_;

private:
  std::list<std::shared_ptr<tile>> tile_list_;
};
} // namespace bombino
