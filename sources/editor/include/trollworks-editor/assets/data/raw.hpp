#pragma once

#include <filesystem>
#include <fstream>

#include <trollworks-editor/assets/data.hpp>


namespace tw::editor::assets {
  class raw_data final : public asset_data {
    public:
      raw_data(const std::filesystem::path& path);

      virtual void on_inspector() const;

    private:
      std::filesystem::path m_path;
      std::ifstream m_stream;
  };
}
