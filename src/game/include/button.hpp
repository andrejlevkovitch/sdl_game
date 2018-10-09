// button.hpp

#pragma once

#include "abstract_object.hpp"

#include <functional>

namespace deep_space {
using callback = std::function<void(void)>;

class button : public levi::abstract_object {
public:
  button(levi::params params, callback callback);
  void hover(bool state);
  void push();
  void update() override;

private:
  callback callback_;
  bool state_;
};
} // namespace deep_space
