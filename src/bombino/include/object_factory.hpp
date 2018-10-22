// object_factory.hpp

#pragma once

#include <functional>
#include <map>
#include <memory>

#include "abstract_object.hpp"
#include "button.hpp"
#include "scene.hpp"

namespace bombino {
using callback_map = std::map<std::string, levi::callback>;

/**\brief parse xml file and push objects in item_list
 * \param state_file xml file with
 * \param state_name parsing state
 * \param itemp_list abstract_object list from scene
 * \param callback_map pointer to map of callback functions, not bounded param
 * \exception throw exception if bounded attribute not recieved*/
levi::item_list parse_state(const std::string &state_file,
                            const std::string &state_name,
                            callback_map *callback_map = nullptr);
}; // namespace bombino
