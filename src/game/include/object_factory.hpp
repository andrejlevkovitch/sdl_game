// object_factory.hpp

#pragma once

#include <functional>
#include <map>
#include <memory>

#include "abstract_object.hpp"
#include "button.hpp"
#include "scene.hpp"

namespace deep_space {
using callback_map = std::map<std::string, callback>;

/**\brief parse xml file and push objects in item_list
 * \param state_file xml file with
 * \param state_name parsing state
 * \param itemp_list abstract_object list from scene
 * \param callback_map pointer to map of callback functions, not bounded param
 * \exception throw exception if bounded attribute not recieved*/
void parse_state(const std::string &state_file, const std::string &state_name,
                 levi::item_list &item_list,
                 callback_map *callback_map = nullptr);

/**\brief created objects by type
 * \param type type of created object
 * \param file_name file name width way to it
 * \param size size of frame
 * \param pos top-left corner of texture
 * \param callback callback function, not bounded param
 * \return smart pointer to created object
 * \exception if type not allowed it throw out_of_range exception*/
std::shared_ptr<levi::abstract_object>
create_object(std::string type, const std::string &file_name, levi::size size,
              levi::vector2d pos, callback callback = nullptr);
}; // namespace deep_space
