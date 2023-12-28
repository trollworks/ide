#pragma once

#include <unordered_map>
#include <filesystem>
#include <memory>

#include <trollworks-editor/userprefs.hpp>
#include <trollworks-editor/data/scene.hpp>


namespace tw::editor {
  struct project {
    std::filesystem::path path;
    user_preferences userprefs;
    data::scene current_scene;
    std::unordered_map<std::string, data::component_factory> component_types;
  };

  class project_manager {
    public:
      static project_manager& instance();

      void parse_cli_args(int argc, char** argv);
      void load(const std::filesystem::path& project_path);
      void save();

      void on_startup();

      project& current_project();

    private:
      project_manager() = default;

      static std::unique_ptr<project_manager> m_instance;
      std::unique_ptr<project> m_project;

      friend std::unique_ptr<project_manager> std::make_unique<project_manager>();
  };
}
