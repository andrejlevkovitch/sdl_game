// object_factory.cpp

#include "object_factory.hpp"

#include <algorithm>
#include <stdexcept>
#include <tinyxml.h>

#include "button.hpp"
#include "gamer.hpp"
#include "object_manager.hpp"

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

    std::string alias;
    attribute_pointer = i->Attribute("alias");
    if (!attribute_pointer) {
      throw std::domain_error{"attribute alias for object with type " + type +
                              " in state " + state_name + " not founded"};
    } else {
      alias = attribute_pointer;
    }

    int x{};
    int y{};
    i->Attribute("x", &x);
    i->Attribute("y", &y);

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

    levi::vector2d pos(x, y);
    try {
      auto &obj_params = object_manager::instance().get_obj_params(alias);
      std::shared_ptr<levi::abstract_object> object;
      if (type == "button") {
        object = std::make_shared<levi::button>(
            obj_params.texture_id, obj_params.object_size, pos, callback);
        item_list.push_back(object);
      }
      if (type == "gamer") {
        bombino::object_type type;
        if (alias == "gamer1") {
          type = bombino::object_type::gamer1;
        } else if (alias == "gamer2") {
          type = bombino::object_type::gamer2;
        }
        auto temp = std::make_shared<bombino::gamer>(
            obj_params.texture_id, obj_params.object_size, pos, type);
        std::pair<uint8_t, uint8_t> front_frames{obj_params.front_frame,
                                                 obj_params.back_frame};
        std::pair<uint8_t, uint8_t> back_frames{obj_params.back_frame,
                                                obj_params.side_frame};
        std::pair<uint8_t, uint8_t> side_frames{obj_params.side_frame,
                                                obj_params.frame_count};
        temp->specify_frame_collection(front_frames, back_frames, side_frames);
        temp->set_texture_width(obj_params.texture_width);
        object = temp;
        item_list.push_back(object);
      }
    } catch (std::exception &) {
      throw;
    }
  }
  return item_list;
}
