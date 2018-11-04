// menu_imgui.cpp

#include "menu_imgui.hpp"
#include "imgui.h"

levi::menu_imgui &levi::menu_imgui::instance() {
  static menu_imgui retval{};
  return retval;
}

levi::menu_imgui::operator bool() { return show_; }

levi::menu_imgui &levi::menu_imgui::operator=(bool b) {
  show_ = b;
  return *this;
}

levi::menu_imgui::menu_imgui() : show_{false} {};
