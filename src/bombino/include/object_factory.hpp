// object_factory.hpp

#pragma once

#include "abstract_object.hpp"
#include "button.hpp"
#include "scene.hpp"
#include <functional>
#include <map>
#include <memory>

namespace bombino {
class callback_map;

/**\brief parse xml file and push objects in item_list
 * \param state_file xml file with
 * \param state_name parsing state
 * \param itemp_list abstract_object list from scene
 * \param callback_map pointer to map of callback functions
 * \exception throw exception if bounded attribute not recieved*/
levi::item_list parse_state(const std::string &state_file,
                            const std::string &state_name,
                            bombino::callback_map *callback_map = nullptr);
}; // namespace bombino
