// object_manager.cpp

#include "object_manager.hpp"
#include "tinyxml2.h"
#include <stdexcept>

bombino::object_params::object_params()
    : texture_width{}, front_frame{}, back_frame{}, side_frame{},
      frame_count{} {}

bombino::object_params::object_params(levi::size object_size_,
                                      const std::string &texture_id_,
                                      unsigned texture_width_,
                                      uint8_t front_frame_, uint8_t back_frame_,
                                      uint8_t side_frame_, uint8_t frame_count_)
    : texture_id{texture_id_}, object_size{object_size_},
      texture_width{texture_width_}, front_frame{front_frame_},
      back_frame{back_frame_}, side_frame{side_frame_}, frame_count{
                                                            frame_count_} {}

bombino::object_manager &bombino::object_manager::instance() {
  static object_manager retval{};
  return retval;
}

unsigned bombino::object_manager::parse_file(const std::string &file_name) {
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error{"object_manager couldn't load file: " + file_name};
  }

  auto root = doc.RootElement();
  if (!root) {
    throw std::runtime_error{"couldn't find root in file: " + file_name};
  }

  unsigned count_load_objects{};
  for (auto element = root->FirstChildElement(); element != nullptr;
       element = element->NextSiblingElement()) {
    std::string alias = element->Attribute("alias");
    if (alias.empty()) {
      continue;
    }

    bombino::object_params obj{};
    obj.texture_id = element->Attribute("texture_id");
    obj.object_size.width = element->IntAttribute("width");
    obj.object_size.height = element->IntAttribute("height");
    obj.texture_width = element->IntAttribute("texture_width");
    obj.frame_count = element->IntAttribute("frame_count");
    obj.front_frame = element->IntAttribute("front_frame");
    obj.back_frame = element->IntAttribute("back_frame");
    obj.side_frame = element->IntAttribute("side_frame");

    obj_map_[alias] = obj;
    ++count_load_objects;
  }
  return count_load_objects;
}

const bombino::object_params &
bombino::object_manager::get_obj_params(const std::string &alias) {
  try {
    return obj_map_.at(alias);
  } catch (std::out_of_range &except) {
    throw std::out_of_range{std::string{"in object_manager: "} + except.what() +
                            " for alias: " + alias};
  }
}

bombino::object_manager::object_manager() {}
