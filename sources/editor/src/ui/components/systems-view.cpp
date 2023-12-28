#include <imgui.h>
#include <imgui/imgui_internal.h>


#include <trollworks-editor/ui/components/systems-view.hpp>


namespace tw::editor::ui {
  systems_view::systems_view(
    ImGuiID& dsid,
    bool& show,
    selection_type& sel_state
  ) : m_dsid(dsid),
      m_show(show),
      m_sel_state(sel_state)
  {}

  void systems_view::render(hooks& h) {
    if (!m_show) {
      return;
    }

    auto shown = ImGui::Begin(
      "Systems##win-view-systems",
      &m_show
    );

    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      ImGui::Text("Systems");
    }

    ImGui::End();
  }
}
