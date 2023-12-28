#include <fmt/format.h>
#include <imgui.h>
#include <SDL.h>
#include <nfd.h>

#include <trollworks-editor/ui/components/menubar.hpp>
#include <trollworks-editor/events.hpp>
#include <trollworks-editor/project.hpp>
#include <trollworks-editor/logger.hpp>


namespace tw::editor::ui {
  menubar::menubar(view_state& state) : m_state(state) {}

  void menubar::render(hooks& h) {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New project")) {
          ;
        }

        if (ImGui::MenuItem("Load project")) {
          nfdchar_t* path = NULL;
          auto result = NFD_PickFolder(NULL, &path);

          switch (result) {
            case NFD_OKAY: {
              auto project_path = std::string(path);
              project_manager::instance().load(project_path);
              break;
            }

            case NFD_CANCEL:
              break;

            default: {
              auto msg = fmt::format("{}", NFD_GetError());
              logger::instance().log(loglevel::error, msg);
              events::instance().redraw();
              break;
            }
          }
        }

        if (ImGui::MenuItem("Save project")) {
          project_manager::instance().save();
          events::instance().redraw();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Build")) {
          ;
        }

        if (ImGui::MenuItem("Build and run")) {
          ;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit")) {
          events::instance().quit();
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Preferences")) {
          ;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Project settings")) {
          ;
        }

        if (ImGui::MenuItem("Build settings")) {
          ;
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Assets")) {
        if (ImGui::MenuItem("Import asset")) {
          ;
        }

        if (ImGui::MenuItem("Create custom asset type")) {
          ;
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Window")) {
        if (ImGui::Checkbox("Entities##toggle-view-entities", &m_state.show_entities)) {
          events::instance().redraw();
        }

        if (ImGui::Checkbox("Systems##toggle-view-systems", &m_state.show_systems)) {
          events::instance().redraw();
        }

        if (ImGui::Checkbox("Scene##toggle-view-scene", &m_state.show_scene)) {
          events::instance().redraw();
        }

        if (ImGui::Checkbox("Assets##toggle-view-assets", &m_state.show_assets)) {
          events::instance().redraw();
        }

        if (ImGui::Checkbox("Inspector##toggle-view-inspector", &m_state.show_inspector)) {
          events::instance().redraw();
        }

        if (ImGui::Checkbox("Console##toggle-view-console", &m_state.show_console)) {
          events::instance().redraw();
        }

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
  }
}
