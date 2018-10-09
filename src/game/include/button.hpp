// button.hpp

#pragma once

#include "abstract_object.hpp"

#include <functional>
#include <memory>

namespace deep_space {
using callback = std::function<void(void)>;

class button : public levi::abstract_object {
public:
  button(const std::string &file_name, callback callback);
  button(levi::params params, std::function<void(void)> callback);
  void hover(bool state);
  void push();
  void update() override;

private:
  std::function<void(void)> callback_;
  bool state_;
};

class button_creator {
public:
  std::shared_ptr<button> create_button(const std::string &alias);
};
} // namespace deep_space
