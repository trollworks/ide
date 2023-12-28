#include <imgui.h>
#include <imgui/imgui_internal.h>

#include <trollworks-editor/ui/components/dockspace.hpp>

namespace tw::editor::ui {
  dockspace::dockspace(dockspace_state& state) : m_state(state) {}

  void dockspace::render(hooks& h) {
    auto dockspace_id = ImGui::GetID("Dockspace");
    ImGui::DockSpace(dockspace_id, {0, 0}, ImGuiDockNodeFlags_None);
    auto root_node = ImGui::DockBuilderGetNode(dockspace_id);

    if (root_node != nullptr && !root_node->IsSplitNode()) {
      ImGuiID pane_left, tmp_node;

      ImGui::DockBuilderSplitNode(
        dockspace_id,
        ImGuiDir_Left,
        0.2f,
        &pane_left,
        &tmp_node
      );
      ImGui::DockBuilderSplitNode(
        tmp_node,
        ImGuiDir_Right,
        0.2f,
        &m_state.dsid_inspector,
        &tmp_node
      );
      ImGui::DockBuilderSplitNode(
        tmp_node,
        ImGuiDir_Down,
        0.2f,
        &m_state.dsid_console,
        &m_state.dsid_scene
      );
      ImGui::DockBuilderSplitNode(
        pane_left,
        ImGuiDir_Up,
        0.5f,
        &m_state.dsid_ecs,
        &m_state.dsid_assets
      );

      ImGui::DockBuilderFinish(dockspace_id);
    }
  }
}
