// texture_manager.cpp

#include "texture_manager.hpp"
#include "engine.hpp"
#include "gl_loader.hpp"
#include "picopng.hpp"
#include "tinyxml2.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <stdexcept>

struct image {
  std::vector<unsigned char> data;
  size_t width;
  size_t height;
};

levi::texture_manager::texture_manager() {}

levi::texture_manager::~texture_manager() {
  for (auto &i : texture_map_) {
    ::glDeleteTextures(1, &i.second.gl_tex);
  }
}

size_t levi::texture_manager::parse_textures(const std::string &texture_file) {
  tinyxml2::XMLDocument xml_doc;
  if (xml_doc.LoadFile(texture_file.c_str()) != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error{"parser can't load file " + texture_file};
  }

  auto root = xml_doc.RootElement();
  if (!root) {
    throw std::runtime_error{"parser can't get root element in file " +
                             texture_file};
  }

  std::string path_to_file =
      texture_file.substr(0, texture_file.find_last_of("/\\") + 1);

  size_t capacity{};
  for (auto i = root->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    std::string texture_id;
    std::string texture_file;

    texture_id = i->Attribute("texture_id");

    texture_file = i->Attribute("file");

    if (texture_map_.find(texture_id) == texture_map_.end()) {
      if (create_texture(texture_id, path_to_file + texture_file)) {
        ++capacity;
      }
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

std::list<std::string> levi::texture_manager::get_not_load_textures() {
  std::list<std::string> retval;
  not_loaded_.swap(retval);
  return retval;
}

bool levi::texture_manager::create_texture(const std::string &texture_id,
                                           const std::string &texture_file) {
  if (!texture_id.empty() && !texture_file.empty()) {
    image image{};
    std::vector<unsigned char> buffer;
    loadFile(buffer, texture_file);
    // here if I set in function image.width and image.height mingw report about
    // error
    unsigned long w{}, h{};
    if (decodePNG(image.data, w, h, &buffer[0], buffer.size(), true) != 0) {
      goto fail;
    }

    image.width = w;
    image.height = h;

    GLuint gl_tex{};
    gl_loader::instance().glActiveTexture(GL_TEXTURE7);
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
fail:
  not_loaded_.push_back("texture " + texture_id + " from file " + texture_file +
                        " because: unkorrect id or name");
  return false;
}
