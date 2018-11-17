// tile_loader.cpp

#include "tile_loader.hpp"
#include "base64.h"
#include "tile.hpp"
#include "tinyxml2.h"
#include <stdexcept>
#include <vector>
#include <zlib.h>

bool check_tile_map(const tinyxml2::XMLElement *root) {
  int version = root->IntAttribute("version");
  if (version != 1) {
    return false;
  }

  const char *pointer_to_attribute{};

  pointer_to_attribute = root->Attribute("orientation");
  if (!pointer_to_attribute ||
      pointer_to_attribute != std::string{"orthogonal"}) {
    return false;
  }

  pointer_to_attribute = root->Attribute("renderorder");
  if (!pointer_to_attribute || pointer_to_attribute != std::string{"left-up"}) {
    return false;
  }

  return true;
}

void bombino::tile_loader::parse_tile_map(const std::string &file_name) {
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
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

  map_size_.width = root->IntAttribute("width");
  map_size_.height = root->IntAttribute("height");

  tile_size_.width = root->IntAttribute("tilewidth");
  tile_size_.height = root->IntAttribute("tileheight");

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

  create_tiles();
}

void bombino::tile_loader::read_tileset(const tinyxml2::XMLElement *tileset) {
  image_id_ = tileset->Attribute("name");
  auto image = tileset->FirstChildElement();
  if (!image) {
    throw std::runtime_error{"couldn't read image information"};
  }
  image_file_name_ = image->Attribute("source");
}

void bombino::tile_loader::read_layer(const tinyxml2::XMLElement *layer) {
  auto data = layer->FirstChildElement();
  if (!data) {
    throw std::runtime_error{"data not found"};
  }
  auto encoding = data->Attribute("encoding");
  auto compression = data->Attribute("compression");
  if (encoding != std::string{"base64"} || compression != std::string{"zlib"}) {
    throw std::runtime_error{"unrecognized options encoding | compression"};
  }

  std::string temp = data->GetText();
  auto first = temp.find_first_not_of(" \n");
  auto last = temp.find_last_not_of(" \n");
  auto encoding_data = temp.substr(first, last - first + 1);
  if (encoding_data.empty()) {
    throw std::runtime_error{"data not founded"};
  }

  std::string decoding_data = base64_decode(encoding_data);
  if (decoding_data.empty()) {
    throw std::runtime_error{"fail of decode data"};
  }
  layout_.resize(map_size_.width * map_size_.height, 0);
  size_t size = layout_.size() * sizeof(int);
  if (::uncompress(reinterpret_cast<Bytef *>(&layout_[0]), &size,
                   reinterpret_cast<Bytef *>(&decoding_data[0]),
                   decoding_data.size()) != Z_OK) {
    throw std::runtime_error{"fail of unpack data"};
  }
}

void bombino::tile_loader::create_tiles() {
  for (int i{}; i < map_size_.height; ++i) {
    for (int j{}; j < map_size_.width; ++j) {
      int index = i * map_size_.width + j;
      levi::vector2d pos(tile_size_.width * j, tile_size_.height * i);
      if (!layout_[index]) {
        continue;
      }
      auto type = static_cast<object_type>(object_type::void_block +
                                           layout_[index] - 1);
      tile_list_.push_back(std::make_shared<tile>(image_id_, tile_size_, pos,
                                                  type, layout_[index] - 1));
    }
  }
}

const std::list<std::shared_ptr<bombino::tile>>
bombino::tile_loader::get_tiles() const {
  return tile_list_;
}
