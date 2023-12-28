#include <fstream>

#include <nlohmann/json.hpp>
#include <argparse.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <SDL.h>
#include <nfd.h>

#include <trollworks-editor/project.hpp>
#include <trollworks-editor/assets/db.hpp>

#include <trollworks-editor/data/components/transform.hpp>
#include <trollworks-editor/data/components/sprite.hpp>

using namespace nlohmann::literals;


namespace tw::editor {
  std::unique_ptr<project_manager> project_manager::m_instance = nullptr;

  project_manager& project_manager::instance() {
    if (!m_instance) {
      m_instance = std::make_unique<project_manager>();
    }

    return *m_instance;
  }

  void project_manager::parse_cli_args(int argc, char** argv) {
    auto parser = argparse::ArgumentParser("trollworks-editor", "0.1.0");

    parser.add_argument("-p", "--project")
      .help("Path to a project folder");

    try {
      parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
      fmt::print(stderr, "ERROR: {}\n", err.what());
      fmt::print(stderr, "{}\n", parser.help().str());
      std::exit(1);
    }

    auto project_path = parser.present<std::string>("-p");
    if (!project_path) {
      nfdchar_t* path = NULL;
      auto result = NFD_PickFolder(NULL, &path);

      switch (result) {
        case NFD_OKAY:
          project_path = std::string(path);
          break;

        case NFD_CANCEL:
          fmt::print(stderr, "No project selected.\n");
          std::exit(2);
          break;

        default:
          fmt::print(stderr, "ERROR: {}\n", NFD_GetError());
          std::exit(1);
          break;
      }
    }

    load(*project_path);
  }

  void project_manager::load(const std::filesystem::path& project_path) {
    auto win   = SDL_GL_GetCurrentWindow();
    auto title = fmt::format("Trollworks Editor - {}", project_path.string());
    SDL_SetWindowTitle(win, title.c_str());

    auto asset_dir = project_path / "Assets";
    std::filesystem::create_directories(asset_dir);
    assets::database::instance().set_location(asset_dir);

    auto userprefs_path = project_path / "UserPreferences.json";
    if (!std::filesystem::exists(userprefs_path)) {
      auto f = std::ofstream(userprefs_path);
      f << R"({})"_json << std::endl;
    }

    m_project = std::make_unique<project>();
    m_project->path = project_path;
    m_project->userprefs.load(userprefs_path);

    m_project->component_types.insert({
      std::string("Transform"),
      []() { return std::make_unique<data::transform_component>(); }
    });
    m_project->component_types.insert({
      std::string("Sprite"),
      []() { return std::make_unique<data::sprite_component>(); }
    });
  }

  void project_manager::save() {
    size_t size = 0;
    auto data   = ImGui::SaveIniSettingsToMemory(&size);
    auto layout = std::string(data, size);

    m_project->userprefs.layout(layout);

    auto userprefs_path = m_project->path / "UserPreferences.json";
    m_project->userprefs.dump(userprefs_path);
  }

  void project_manager::on_startup() {
    auto layout = m_project->userprefs.layout();
    if (layout) {
      auto layout_data = layout.value();
      ImGui::LoadIniSettingsFromMemory(layout_data.data(), layout_data.size());
    }
  }

  project& project_manager::current_project() {
    return *m_project;
  }
}
