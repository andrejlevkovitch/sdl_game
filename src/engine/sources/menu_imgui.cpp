// menu_imgui.cpp

#include "menu_imgui.hpp"
#include "imgui.h"

void levi::show_menu_imgui(bool *show) {
  if (*show) {
    auto mouse_pos = ImGui::GetMousePos();
    ImGui::Begin("Menu");

    ImGui::Text("current mouse position:\n %.0f %.0f", mouse_pos.x,
                mouse_pos.y);

    ImGui::End();
  }
}
