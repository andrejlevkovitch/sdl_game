// object_factory.cpp

#include "object_factory.hpp"
#include "button.hpp"
#include "callback_map.hpp"
#include "gamer.hpp"
#include "object_manager.hpp"
#include "tinyxml2.h"
#include <algorithm>
#include <stdexcept>

levi::item_list bombino::parse_state(const std::string &state_file,
                                     const std::string &state_name,
                                     callback_map *callback_map) {
  levi::item_list item_list;
  tinyxml2::XMLDocument xml_doc;
  if (xml_doc.LoadFile(state_file.c_str()) != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error{"xml file " + state_file + " not load"};
  }

  auto root_element = xml_doc.RootElement();

  if (!root_element) {
    throw std::runtime_error{"can't find root element in xml file " +
                             state_file};
  }

  tinyxml2::XMLElement *state{nullptr};

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

    std::string type = i->Attribute("type");

    std::string alias = i->Attribute("alias");

    int x = i->IntAttribute("x");
    int y = i->IntAttribute("y");

    std::string callback_id = i->Attribute("callback_id");

    std::function<void(void)> callback = nullptr;
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
            obj_params.texture_id, obj_params.object_size, pos, type, callback);
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
