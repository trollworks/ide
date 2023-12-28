#include <imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <native-open.hpp>
#include <fmt/format.h>

#include <trollworks-editor/project.hpp>
#include <trollworks-editor/events.hpp>
#include <trollworks-editor/logger.hpp>
#include <trollworks-editor/assets/db.hpp>

#include <trollworks-editor/ui/components/inspector-view.hpp>


namespace tw::editor::ui {
  inspector_view::inspector_view(
    ImGuiID& dsid,
    bool& show,
    selection_type& sel_state
  ) : m_dsid(dsid),
      m_show(show),
      m_sel_state(sel_state)
  {}

  void inspector_view::render(hooks& h) {
    if (!m_show) {
      return;
    }

    auto shown = ImGui::Begin(
      "Inspector##win-view-inspector",
      &m_show,
      ImGuiWindowFlags_MenuBar
    );
    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      std::visit([&](auto sel) { show_selection(sel); }, m_sel_state);
    }

    ImGui::End();
  }

  void inspector_view::show_selection(empty_sel& s) {}

  void inspector_view::show_selection(entity_sel& sel) {
    auto& project = project_manager::instance().current_project();

    if (!project.current_scene.entities.contains(sel.guid)) {
      m_sel_state = selection_type(empty_sel{});
      return;
    }

    auto& entity = project.current_scene.entities.at(sel.guid);
    auto guid = sel.guid.str();

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("+")) {
        for (auto& [name, factory] : project.component_types) {
          if (!entity->components.contains(name)) {
            auto label = fmt::format("Add {}", name);
            if (ImGui::MenuItem(label.c_str())) {
              entity->components.insert({name, factory()});
            }
          }
        }

        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }

    ImGui::SeparatorText("Entity");

    ImGui::InputText(
      "GUID##inspector-entity-guid",
      &guid,
      ImGuiInputTextFlags_ReadOnly
    );

    ImGui::InputText(
      "Name##inspector-entity-name",
      &entity->name
    );

    for (auto& [name, cmp] : entity->components) {
      auto label = fmt::format("Component: {}", name);
      ImGui::SeparatorText(label.c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, {1, 0, 0, 0.5f});
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {1, 0, 0, 1});
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, {1, 0, 0, 0.66f});
      if (ImGui::Button("Delete")) {
        events::instance().command(
          [entity, name](auto data) {
            entity->components.erase(name);
          },
          nullptr
        );
      }
      ImGui::PopStyleColor(3);

      ImGui::SameLine();

      if (ImGui::Button("Reset")) {
        events::instance().command(
          [entity, name](auto data) {
            auto& project = project_manager::instance().current_project();
            entity->components[name] = project.component_types[name]();
          },
          nullptr
        );
      }

      ImGui::Separator();

      cmp->on_inspector();
    }
  }

  void inspector_view::show_selection(system_sel& sel) {
    ImGui::Text("System %lld", sel.id);
  }

  void inspector_view::show_selection(asset_sel& sel) {
    auto asset_desc = assets::database::instance().get_meta(sel.guid);
    if (asset_desc) {
      if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Open")) {
          try {
            nativeopen::open_file(asset_desc->fullpath());
          }
          catch (const std::runtime_error& err) {
            logger::instance().log(
              loglevel::error,
              fmt::format(
                "Could not open asset '{}': {}",
                asset_desc->fullpath().string(),
                err.what()
              )
            );
          }
        }

        ImGui::EndMenuBar();
      }

      ImGui::SeparatorText("Asset");

      auto guid = asset_desc->guid().str();
      ImGui::InputText(
        "GUID##inspector-asset-guid",
        &guid,
        ImGuiInputTextFlags_ReadOnly
      );

      auto fullpath = asset_desc->fullpath().string();
      ImGui::InputText(
        "Path##inspector-asset-path",
        &fullpath,
        ImGuiInputTextFlags_ReadOnly
      );

      try {
        auto asset_data = assets::database::instance().get_data(*asset_desc);
        if (asset_data) {
          asset_data->on_inspector();
        }
      }
      catch (const std::runtime_error& err) {
        logger::instance().log(loglevel::error, fmt::format("{}", err.what()));
      }
    }
    else {
      ImGui::Text("Missing asset");
    }
  }
}
