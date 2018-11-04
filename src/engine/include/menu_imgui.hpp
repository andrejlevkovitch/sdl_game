// menu_imgui.hpp

namespace levi {
class menu_imgui {
public:
  static menu_imgui &instance();
  operator bool();
  menu_imgui &operator=(bool);

private:
  menu_imgui();
  menu_imgui(const menu_imgui &);
  menu_imgui &operator=(const menu_imgui &);

  bool show_;
};
}; // namespace levi
