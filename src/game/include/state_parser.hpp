// state_parser.hpp

#pragma once

#include <functional>
#include <map>
#include <string>

#include "button.hpp"
#include "scene.hpp"

namespace deep_space {
using callback_map = std::map<std::string, callback>;

class state_parser {
public:
  void parse_state(
      const std::string &state_file, const std::string &state_name,
      levi::item_list &item_list,
      std::map<std::string, std::function<void(void)>> *callback_map = nullptr);

private:
};
}; // namespace deep_space
