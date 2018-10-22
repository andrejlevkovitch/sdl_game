// object_factory.cpp

#include "object_factory.hpp"

#include <algorithm>
#include <stdexcept>
#include <tinyxml.h>

#include "button.hpp"
#include "gamer.hpp"

levi::item_list bombino::parse_state(
    const std::string &state_file, const std::string &state_name,
    std::map<std::string, std::function<void(void)>> *callback_map) {
  levi::item_list item_list;
  ::TiXmlDocument xml_doc;
  if (!xml_doc.LoadFile(state_file)) {
    throw std::runtime_error{"xml file " + state_file + " not load"};
  }

  std::string way_to_files{state_file};
  auto last_slesh = std::find(way_to_files.rbegin(), way_to_files.rend(), '/');
  if (last_slesh == way_to_files.rend()) {
    way_to_files.clear();
  } else {
    way_to_files.erase(last_slesh.base(), way_to_files.rbegin().base());
  }

  auto root_element = xml_doc.RootElement();

  if (!root_element) {
    throw std::runtime_error{"can't find root element in xml file " +
                             state_file};
  }

  ::TiXmlElement *state{nullptr};

  for (auto i = root_element->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    if (i->Value() == state_name) {
      state = i;
      break;
    }
  }

  if (!state) {
    throw std::runtime_error{"can't find state " + state_name +
                             " in xml file " + state_file};
  }

  for (auto i = state->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    const char *attribute_pointer{};

    std::string type;
    if ((attribute_pointer = i->Attribute("type"))) {
      type = attribute_pointer;
    } else {
      throw std::domain_error{"attribute type for object in state " +
                              state_name + " not founded"};
    }

    std::string alias = i->Attribute("alias");

    int x{};
    int y{};
    int height{};
    int width{};
    i->Attribute("x", &x);
    i->Attribute("y", &y);
    i->Attribute("width", &width);
    i->Attribute("height", &height);

    frames front_frames;
    frames side_frames;
    frames back_frames;
    int first_frame{};
    int second_frame{};
    i->Attribute("front_frame", &first_frame);
    i->Attribute("front_frame_count", &second_frame);
    front_frames.first = first_frame;
    front_frames.second = second_frame;
    i->Attribute("side_frame", &first_frame);
    i->Attribute("side_frame_count", &second_frame);
    side_frames.first = first_frame;
    side_frames.second = second_frame;
    i->Attribute("back_frame", &first_frame);
    i->Attribute("back_frame_count", &second_frame);
    back_frames.first = first_frame;
    back_frames.second = second_frame;

    levi::size texture_size;
    i->Attribute("texture_width", &texture_size.width);
    i->Attribute("texture_height", &texture_size.height);

    std::string texture_id;
    if ((attribute_pointer = i->Attribute("texture_id"))) {
      texture_id = attribute_pointer;
    } else {
      throw std::domain_error{"attribute texture_id for object in state " +
                              state_name + " not founded"};
    }

    std::string callback_id;
    if ((attribute_pointer = i->Attribute("callback_id"))) {
      callback_id = attribute_pointer;
    }

    levi::callback callback = nullptr;
    if (callback_map && !callback_id.empty()) {
      try {
        callback = callback_map->at(callback_id);
      } catch (std::exception &) {
        throw std::out_of_range{state_name + " not supported callback " +
                                callback_id};
      }
    }

    levi::size size{width, height};
    levi::vector2d pos(x, y);
    try {
      std::shared_ptr<levi::abstract_object> object;
      if (type == "button") {
        object =
            std::make_shared<levi::button>(texture_id, size, pos, callback);
      }
      if (type == "gamer") {
        bombino::object_type type;
        if (alias == "gamer1") {
          type = bombino::object_type::gamer1;
        }
        if (alias == "gamer2") {
          type = bombino::object_type::gamer2;
        }
        auto temp =
            std::make_shared<bombino::gamer>(texture_id, size, pos, type);
        temp->specify_frame_collection(front_frames, side_frames, back_frames);
        temp->set_texture_size(texture_size);
        object = temp;
      }
      item_list.push_back(object);
    } catch (std::exception) {
      throw;
    }
  }
  return item_list;
}
