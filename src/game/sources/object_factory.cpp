// object_factory.cpp

#include "object_factory.hpp"

#include <stdexcept>
#include <tinyxml.h>

#include "button.hpp"
#include "shiep.hpp"

void deep_space::parse_state(
    const std::string &state_file, const std::string &state_name,
    levi::item_list &item_list,
    std::map<std::string, std::function<void(void)>> *callback_map) {
  ::TiXmlDocument xml_doc;
  if (!xml_doc.LoadFile(state_file.c_str())) {
    throw std::runtime_error{"xml file " + state_file + " not load"};
  }

  std::string way{state_file};
  /// todo this cikle is danger!
  for (auto i = way.end() - 1; i != way.begin(); --i) {
    if (*i == '/' || *i == '\\') {
      way.erase(i + 1, way.end());
      break;
    }
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

    int x{};
    int y{};
    int height{};
    int width{};
    i->Attribute("x", &x);
    i->Attribute("y", &y);
    i->Attribute("width", &width);
    i->Attribute("height", &height);

    std::string texture = way;
    if ((attribute_pointer = i->Attribute("texture"))) {
      texture += attribute_pointer;
    } else {
      throw std::domain_error{"attribute texture for object in state " +
                              state_name + " not founded"};
    }

    std::string callback_id;
    if ((attribute_pointer = i->Attribute("callback_id"))) {
      callback_id = attribute_pointer;
    }

    callback callback = nullptr;
    if (callback_map && !callback_id.empty()) {
      try {
        callback = callback_map->at(callback_id);
      } catch (std::exception &) {
        throw std::out_of_range{state_name + " not supported callback " +
                                callback_id};
      }
    }

    try {
      auto object = create_object(type, texture, levi::size{width, height},
                                  levi::vector2d(x, y), callback);
      item_list.push_back(object);
    } catch (std::exception) {
      throw;
    }
  }
}
std::shared_ptr<levi::abstract_object>
deep_space::create_object(std::string type, const std::string &file_name,
                          levi::size size, levi::vector2d pos,
                          std::function<void(void)> callback) {
  if (type == "button") {
    return std::make_shared<deep_space::button>(file_name, size, pos, callback);
  }
  if (type == "gamer") {
    return std::make_shared<deep_space::shiep>(file_name, size, pos);
  }
  throw std::out_of_range{"can't create object with type " + type};
}
