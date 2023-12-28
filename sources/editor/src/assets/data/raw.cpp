#include <imgui.h>

#include <trollworks-editor/assets/data/raw.hpp>


namespace tw::editor::assets {
  raw_data::raw_data(const std::filesystem::path& path)
    : m_path(path), m_stream(path)
  {}

  void raw_data::on_inspector() const {}
}
