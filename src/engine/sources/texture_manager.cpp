// texture_manager.cpp

#include "texture_manager.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <tinyxml.h>

#include "engine.hpp"

levi::texture_manager::texture_manager(engine &engine) : engine_{engine} {}

levi::texture_manager::~texture_manager() {
  for (auto &i : texture_map_) {
    SDL_DestroyTexture(i.second);
  }
}

size_t levi::texture_manager::parse_textures(const std::string &texture_file) {
  ::TiXmlDocument xml_doc;
  if (!xml_doc.LoadFile(texture_file.c_str())) {
    throw std::runtime_error{"parser can't load file " + texture_file};
  }

  auto root = xml_doc.RootElement();
  if (!root) {
    throw std::runtime_error{"parser can't get root element in file " +
                             texture_file};
  }

  std::string way_to_files = texture_file;
  for (auto i = way_to_files.rbegin(); i != way_to_files.rend(); ++i) {
    if (*i == '\\' || *i == '/') {
      way_to_files.erase(i.base(), way_to_files.rbegin().base());
      break;
    }
  }

  size_t capacity{};
  for (auto i = root->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    const char *pointer{};
    std::string texture_id;
    std::string texture_file;

    if ((pointer = i->Attribute("texture_id"))) {
      texture_id = pointer;
    }

    if ((pointer = i->Attribute("file"))) {
      texture_file = way_to_files + pointer;
    }

    if (!texture_id.empty() && !texture_file.empty()) {
      auto texture = engine_.create_texture(texture_file);
      if (texture) {
        texture_map_[texture_id] = texture;
        ++capacity;
      }
    }
  }

  return capacity;
}

SDL_Texture *levi::texture_manager::get_texture(const std::string &texture_id) {
  try {
    return texture_map_.at(texture_id);
  } catch (std::out_of_range &) {
    return nullptr;
  }
}
