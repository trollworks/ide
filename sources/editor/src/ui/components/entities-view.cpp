#include <imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <fmt/format.h>

#include <trollworks-editor/events.hpp>
#include <trollworks-editor/project.hpp>

#include <trollworks-editor/ui/components/entities-view.hpp>


namespace tw::editor::ui {
  entities_view::entities_view(
    ImGuiID& dsid,
    bool& show,
    selection_type& sel_state
  ) : m_dsid(dsid),
      m_show(show),
      m_sel_state(sel_state)
  {}

  void entities_view::render(hooks& h) {
    auto& search = h.use_state<std::string>();

    if (!m_show) {
      return;
    }

    auto shown = ImGui::Begin(
      "Entities##win-view-entities",
      &m_show,
      ImGuiWindowFlags_MenuBar
    );
    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("+")) {
          events::instance().command(
            [](auto data) {
              auto& project = project_manager::instance().current_project();

              auto guid = xg::newGuid();
              auto entity = std::make_shared<data::entity>(data::entity{
                .guid = guid,
                .name = "Entity"
              });
              project.current_scene.entities.insert({ guid, entity });
            },
            nullptr
          );
        }

        ImGui::EndMenuBar();
      }

      auto size = ImGui::GetContentRegionAvail();

      ImGui::SetNextItemWidth(size.x);
      ImGui::InputTextWithHint(
        "##entities-search",
        "Search entities...",
        &search,
        ImGuiInputTextFlags_EscapeClearsAll
      );

      ImGui::Separator();

      auto& project = project_manager::instance().current_project();

      for (auto& [guid, entity] : project.current_scene.entities) {
        if (!search.empty() && entity->name.find(search) == std::string::npos) {
          continue;
        }

        bool is_selected = m_sel_state == entity_sel{.guid = guid };

        ImGui::PushStyleColor(ImGuiCol_Button, {1, 0, 0, 0.5f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {1, 0, 0, 1});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {1, 0, 0, 0.66f});

        ImGui::BeginGroup();

        auto del_label = fmt::format("×##delete-entity-{}", guid.str());
        if (ImGui::Button(del_label.c_str())) {
          events::instance().command(
            [guid](auto data) {
              auto& project = project_manager::instance().current_project();
              project.current_scene.entities.erase(guid);
            },
            nullptr
          );
        }

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        auto sel_label = fmt::format("{}##select-entity-{}", entity->name, guid.str());
        if (
          ImGui::Selectable(sel_label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick)
          && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)
        ) {
          m_sel_state = selection_type(entity_sel{.guid = guid});
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }

        ImGui::EndGroup();
      }
    }

    ImGui::End();
  }
}
