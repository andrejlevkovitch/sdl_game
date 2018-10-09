// state_parser.cpp

#include "state_parser.hpp"
#include "object_factory.hpp"

#include <stdexcept>

#include <tinyxml.h>

void deep_space::state_parser::parse_state(
    const std::string &state_file, const std::string &state_name,
    levi::item_list &item_list,
    std::map<std::string, std::function<void(void)>> *callback_map) {
  ::TiXmlDocument xml_doc;
  if (!xml_doc.LoadFile(state_file.c_str())) {
    throw std::runtime_error{"xml file " + state_file + " not load"};
  }

  std::string way{state_file};
  /// todo this cikle id danger!
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
    std::string type = i->Attribute("type");
    std::string alias = i->Attribute("alias");
    int x{};
    int y{};
    int height{};
    int width{};
    i->Attribute("x", &x);
    i->Attribute("y", &y);
    i->Attribute("width", &width);
    i->Attribute("height", &height);
    std::string texture = way + i->Attribute("texture");

    std::cerr << texture << std::endl;

    std::string callback_id;
    if (i->Attribute("callback_id")) {
      callback_id = i->Attribute("callback_id");
    }
    std::cerr << "first\n";
    callback callback = nullptr;
    std::cerr << "second\n";
    if (callback_map && !callback_id.empty()) {
      try {
        callback = callback_map->at(callback_id);
      } catch (std::exception &) {
        throw std::out_of_range{state_name + " not supported callback " +
                                callback_id};
      }
    }

    levi::params params = std::make_tuple(texture, levi::size{width, height},
                                          levi::vector2d(x, y));

    try {
      auto object = create_object(type, params, callback);
      std::cerr << "create object\n";
      item_list.push_back(object);
    } catch (std::exception) {
      throw;
    }
  }
}
