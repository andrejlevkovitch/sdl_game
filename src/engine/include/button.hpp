// button.hpp

#pragma once

#include "abstract_object.hpp"
#include <functional>

namespace levi {
class button : public levi::abstract_object {
public:
  button(const std::string &texture_id, levi::size, levi::vector2d pos,
         std::function<void(void)> callback);
  void hover(bool state);
  void push();
  void update() override;

private:
  std::function<void(void)> callback_;
  bool state_;
};
} // namespace levi
