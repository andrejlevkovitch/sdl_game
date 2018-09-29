// button.hpp

#include "abstract_object.hpp"

#include <functional>

namespace deep_space {
class button : public levi::abstract_object {
public:
  button(const std::string &file_name, std::function<void(void)> callback);
  void hover(bool state);
  void push();
  void update() override;

private:
  std::function<void(void)> callback_;
  bool state_;
};
} // namespace deep_space
