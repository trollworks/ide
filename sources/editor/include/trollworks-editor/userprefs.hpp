#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>


namespace tw::editor {
  class user_preferences {
    public:
      void load(const std::filesystem::path& path);
      void dump(const std::filesystem::path& path) const;

      void layout(const std::string &data);
      std::optional<std::string> layout() const;

    private:
      nlohmann::json m_data;
  };
}
