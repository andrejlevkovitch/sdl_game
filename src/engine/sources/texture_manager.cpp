// texture_manager.cpp

#include "texture_manager.hpp"

#include <algorithm>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <tinyxml.h>

#include "engine.hpp"
#include "gl_loader.hpp"
#include "load_png.hpp"

levi::texture_manager::texture_manager() {}

levi::texture_manager::~texture_manager() {
  for (auto &i : texture_map_) {
    ::glDeleteTextures(1, &i.second.gl_tex);
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
  auto last_slesh = std::find(way_to_files.rbegin(), way_to_files.rend(), '/');
  if (last_slesh == way_to_files.rend()) {
    way_to_files.clear();
  } else {
    way_to_files.erase(last_slesh.base(), way_to_files.rbegin().base());
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

    if (create_texture(texture_id, texture_file)) {
      ++capacity;
    }
  }

  return capacity;
}

levi::texture
levi::texture_manager::get_texture(const std::string &texture_id) const {
  try {
    return texture_map_.at(texture_id);
  } catch (std::out_of_range &) {
    throw;
  }
}

std::list<std::string> levi::texture_manager::get_not_load_objects() {
  std::list<std::string> retval;
  not_loaded_.swap(retval);
  return retval;
}

bool levi::texture_manager::create_texture(const std::string &texture_id,
                                           const std::string &texture_file) {
  if (!texture_id.empty() && !texture_file.empty()) {
    image image{};
    try {
      image = load_png_as_rgba(texture_file);
    } catch (std::exception &except) {
      not_loaded_.push_back("texture " + texture_id + " from file " +
                            texture_file + " because: " + except.what());
      return false;
    }

    GLuint gl_tex{};
    ::glActiveTexture(GL_TEXTURE7);
    LEVI_CHECK();
    ::glGenTextures(1, &gl_tex);
    LEVI_CHECK();
    ::glBindTexture(GL_TEXTURE_2D, gl_tex);
    LEVI_CHECK();

    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, &image.data[0]);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    LEVI_CHECK();

    texture_map_[texture_id] = texture{gl_tex, image.width, image.height};
    return true;
  }
  return false;
}
