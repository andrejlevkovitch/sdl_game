// button.hpp

#include "abstract_object.hpp"

namespace deep_space {
class button : public levi::abstract_object {
public:
  button(const std::string &file_name, void (*callback)(void));
  void hover(bool state);
  void push();
  void update() override;

private:
  void (*callback_)(void);
  bool state_;
};
} // namespace deep_space
