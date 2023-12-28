#pragma once

#include <filesystem>

#include <crossguid/guid.hpp>
#include <nlohmann/json.hpp>

#include <trollworks-editor/assets/type.hpp>


namespace tw::editor::assets {
  class asset_descriptor {
    public:
      xg::Guid guid() const;
      const std::filesystem::path& fullpath() const;
      const std::filesystem::path& relpath() const;
      nlohmann::json attributes() const;
      asset_type type() const;

      void save() const;
      void erase() const;

      static asset_descriptor empty(asset_type type);

      static asset_descriptor from_path(
        const std::filesystem::path& asset_dir,
        const std::filesystem::path& relpath
      );

    private:
      asset_descriptor() = default;

    private:
      xg::Guid m_guid;
      std::filesystem::path m_fullpath;
      std::filesystem::path m_relpath;
      nlohmann::json m_attrs;
      asset_type m_type;
  };
}
