// button.hpp

#pragma once

#include "abstract_object.hpp"

namespace levi {
class button : public levi::abstract_object {
public:
  button(const std::string &texture_id, levi::size, levi::vector2d pos,
         callback callback);
  void hover(bool state);
  void push();
  void update() override;

private:
  callback callback_;
  bool state_;
};
} // namespace levi
