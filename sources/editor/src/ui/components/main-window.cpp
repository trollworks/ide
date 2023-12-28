#include <imgui.h>

#include <trollworks-editor/ui/state/dockspace.hpp>
#include <trollworks-editor/ui/state/selection.hpp>
#include <trollworks-editor/ui/state/view.hpp>

#include <trollworks-editor/ui/components/main-window.hpp>
#include <trollworks-editor/ui/components/dockspace.hpp>
#include <trollworks-editor/ui/components/menubar.hpp>
#include <trollworks-editor/ui/components/entities-view.hpp>
#include <trollworks-editor/ui/components/systems-view.hpp>
#include <trollworks-editor/ui/components/assets-view.hpp>
#include <trollworks-editor/ui/components/scene-view.hpp>
#include <trollworks-editor/ui/components/inspector-view.hpp>
#include <trollworks-editor/ui/components/console-view.hpp>


namespace tw::editor::ui {
  void main_window::render(hooks& h) {
    auto& ds_state  = h.use_state<dockspace_state>();
    auto& view_wins = h.use_state<view_state>();
    auto& sel_state = h.use_state<selection_type>(selection_type(empty_sel{}));

    auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    auto window_flags = 0
      | ImGuiWindowFlags_MenuBar
      | ImGuiWindowFlags_NoDocking
      | ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoNavFocus
      | ImGuiWindowFlags_NoBackground
      ;

    ImGui::Begin("MainWindow", nullptr, window_flags);

    ImGui::PopStyleVar(3);

    h.render<dockspace>(ds_state);
    h.render<menubar>(view_wins);

    ImGui::End();

    h.render<entities_view>(ds_state.dsid_ecs, view_wins.show_entities, sel_state);
    h.render<systems_view>(ds_state.dsid_ecs, view_wins.show_systems, sel_state);
    h.render<assets_view>(ds_state.dsid_assets, view_wins.show_assets, sel_state);
    h.render<scene_view>(ds_state.dsid_scene, view_wins.show_scene, sel_state);
    h.render<inspector_view>(ds_state.dsid_inspector, view_wins.show_inspector, sel_state);
    h.render<console_view>(ds_state.dsid_console, view_wins.show_console);
  }
}
