#pragma once

#include <imgui.h>

namespace tw::editor::ui {
  struct dockspace_state {
    ImGuiID dsid_ecs;
    ImGuiID dsid_assets;
    ImGuiID dsid_scene;
    ImGuiID dsid_inspector;
    ImGuiID dsid_console;
  };
}
