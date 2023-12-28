#include <fstream>

#include <trollworks-editor/userprefs.hpp>

using namespace nlohmann;

namespace tw::editor {
  void user_preferences::load(const std::filesystem::path& path) {
    auto f = std::ifstream(path);
    m_data = json::parse(f);
  }

  void user_preferences::dump(const std::filesystem::path& path) const {
    auto f = std::ofstream(path);
    f << m_data << std::endl;
  }

  void user_preferences::layout(const std::string &data) {
    m_data["layout"] = data;
  }

  std::optional<std::string> user_preferences::layout() const {
    if (!m_data.contains("layout")) {
      return std::nullopt;
    }

    return m_data.at("layout").get<std::string>();
  }
}
