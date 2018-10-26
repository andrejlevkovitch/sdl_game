// callback_map.hpp

#pragma once

#include <map>

#include "dop_classes.hpp"

namespace bombino {
/**\brief singleton, intakes all callbacks functions*/
class callback_map : public std::map<std::string, levi::callback> {
public:
  static callback_map &instance();

private:
  callback_map();
  callback_map(const callback_map &) = delete;
  callback_map &operator=(const callback_map &) = delete;
};
}; // namespace bombino
