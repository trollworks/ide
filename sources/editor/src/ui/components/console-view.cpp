#include <imgui.h>
#include <imgui/imgui_internal.h>

#include <trollworks-editor/logger.hpp>

#include <trollworks-editor/ui/components/console-view.hpp>


namespace tw::editor::ui {
  console_view::console_view(
    ImGuiID& dsid,
    bool& show
  ) : m_dsid(dsid),
      m_show(show)
  {}

  void console_view::render(hooks& h) {
    if (!m_show) {
      return;
    }

    auto shown = ImGui::Begin(
      "Console##win-view-console",
      &m_show,
      ImGuiWindowFlags_MenuBar
    );
    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Clear##win-view-console-clear")) {
          logger::instance().clear();
        }

        ImGui::EndMenuBar();
      }

      if (ImGui::BeginChild("ConsoleLogRegion", {0, 0}, false, ImGuiWindowFlags_HorizontalScrollbar)) {
        auto& l = logger::instance();
        ImGui::TextUnformatted(l.begin(), l.end());

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
          ImGui::SetScrollHereY(1.0f);
        }
      }

      ImGui::EndChild();
    }

    ImGui::End();
  }
}
