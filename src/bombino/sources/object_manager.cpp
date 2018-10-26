// object_manager.cpp

#include "object_manager.hpp"
#include <stdexcept>
#include <tinyxml.h>

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
  ::TiXmlDocument doc;
  if (!doc.LoadFile(file_name)) {
    throw std::runtime_error{"object_manager couldn't load file: " + file_name};
  }

  auto root = doc.RootElement();
  if (!root) {
    throw std::runtime_error{"couldn't find root in file: " + file_name};
  }

  unsigned count_load_objects{};
  for (auto element = root->FirstChildElement(); element != nullptr;
       element = element->NextSiblingElement()) {
    std::string alias;
    int obj_width{};
    int obj_height{};
    std::string texture_id;
    int texture_width{};
    int frame_count{};
    int front_frame{};
    int side_frame{};
    int back_frame{};

    const char *text_ptr{nullptr};

    text_ptr = element->Attribute("alias");
    if (!text_ptr) {
      continue;
    }
    alias = text_ptr;

    text_ptr = element->Attribute("texture_id");
    if (!text_ptr) {
      continue;
    }
    texture_id = text_ptr;

    element->Attribute("width", &obj_width);
    element->Attribute("height", &obj_height);

    element->Attribute("texture_width", &texture_width);

    element->Attribute("frame_count", &frame_count);
    element->Attribute("front_frame", &front_frame);
    element->Attribute("back_frame", &back_frame);
    element->Attribute("side_frame", &side_frame);

    bombino::object_params obj(levi::size{obj_width, obj_height}, texture_id,
                               texture_width, front_frame, back_frame,
                               side_frame, frame_count);
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
