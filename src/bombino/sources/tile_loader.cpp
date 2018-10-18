// tile_loader.cpp

#include "tile_loader.hpp"

#include <stdexcept>
#include <tinyxml.h>
#include <vector>
#include <zlib.h>

#include "base64.h"
#include "engine.hpp"
#include "texture_manager.hpp"

bool check_tile_map(const ::TiXmlElement *root) {
  int version{};
  root->Attribute("version", &version);
  if (version != 1) {
    return false;
  }

  std::string orientation = root->Attribute("orientation");
  if (orientation != "orthogonal") {
    return false;
  }

  std::string renderorder = root->Attribute("renderorder");
  if (renderorder != "left-up") {
    return false;
  }

  return true;
}

void bombino::tile_loader::parse_tile_map(const std::string &file_name) {
  ::TiXmlDocument doc;
  if (!doc.LoadFile(file_name)) {
    throw std::runtime_error{"tile_map file " + file_name +
                             " couldn't be load"};
  }

  auto root = doc.RootElement();
  if (!root) {
    throw std::runtime_error{"couldn't find root in file " + file_name};
  }

  if (!check_tile_map(root)) {
    throw std::runtime_error{"unsupported tile_map"};
  }

  int width{};
  int height{};
  root->Attribute("width", &width);
  root->Attribute("height", &height);
  map_width_ = width;
  map_height_ = height;

  root->Attribute("tilewidth", &width);
  root->Attribute("tileheight", &height);
  tile_width_ = width;
  tile_height_ = height;

  auto tileset = root->FirstChildElement();
  if (!tileset) {
    throw std::runtime_error{"couldn't find tileset in file " + file_name};
  }

  if (tileset->Value() != std::string{"tileset"}) {
    throw std::runtime_error{std::string{"unexpectes value: "} +
                             tileset->Value() + " insted tileset"};
  }
  try {
    read_tileset(tileset);
  } catch (std::exception &) {
    throw;
  }

  auto layer = tileset->NextSiblingElement();
  if (!layer) {
    throw std::runtime_error{"couldn't find layer in file " + file_name};
  }

  if (layer->Value() != std::string{"layer"}) {
    throw std::runtime_error{std::string{"unexpectes value: "} +
                             layer->Value() + " insted layer"};
  }
  try {
    read_layer(layer);
  } catch (std::exception &) {
    throw;
  }
}

std::string bombino::tile_loader::read_tileset(const TiXmlElement *tileset) {
  std::string image_id = tileset->Attribute("name");
  auto image = tileset->FirstChildElement();
  std::string image_file_name = image->Attribute("name");
  int width{};
  int height{};
  image->Attribute("width", &width);
  image->Attribute("height", &height);
  if (levi::engine::instance().texture_manager().create_texture(
          image_id, image_file_name)) {
    return image_id;
  }
  return "";
}

void bombino::tile_loader::read_layer(const TiXmlElement *layer) {
  auto data = layer->FirstChildElement();
  if (!data) {
    throw std::runtime_error{"data not found"};
  }
  std::string encoding = data->Attribute("encoding");
  std::string compression = data->Attribute("compression");
  if (encoding != "base64" || compression != "zlib") {
    throw std::runtime_error{"unrecognized options encoding | compression"};
  }

  std::string encoding_data = data->GetText();
  if (encoding_data.empty()) {
    throw std::runtime_error{"data not founded"};
  }

  std::vector<int> layout(map_width_ * map_height_, 0);
  size_t size = layout.size() * sizeof(int);
  if (::uncompress(reinterpret_cast<Bytef *>(&layout[0]), &size,
                   reinterpret_cast<Bytef *>(&encoding_data),
                   encoding_data.size()) != Z_OK) {
    throw std::runtime_error{"fail of decode data"};
  }
}
