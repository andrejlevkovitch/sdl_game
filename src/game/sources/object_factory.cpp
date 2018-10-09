// object_factory.cpp

#include "object_factory.hpp"

#include <stdexcept>

#include "button.hpp"
#include "shiep.hpp"

#include <iostream>

std::shared_ptr<levi::abstract_object>
deep_space::create_object(std::string type, levi::params params,
                          std::function<void(void)> callback) {
  std::cerr << type << std::endl;
  if (type == "button") {
    return std::make_shared<deep_space::button>(params, callback);
  }
  if (type == "gamer") {
    std::cerr << "create shiep\n";
    return std::make_shared<deep_space::shiep>(params);
  }
  throw std::out_of_range{"can't create object with type " + type};
}
