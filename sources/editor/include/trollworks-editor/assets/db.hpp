#pragma once

#include <unordered_map>
#include <filesystem>
#include <optional>
#include <memory>
#include <vector>
#include <mutex>

#include <trollworks-editor/assets/descriptor.hpp>
#include <trollworks-editor/assets/data.hpp>

#include <trollworks-editor/assets/loader.hpp>
#include <trollworks-editor/assets/watch.hpp>

namespace tw::editor::assets {
  class database {
    public:
      static database& instance();

      void set_location(const std::filesystem::path& asset_dir);

      std::optional<asset_descriptor> get_meta(xg::Guid guid);
      std::vector<asset_descriptor> get_all_meta();
      std::shared_ptr<asset_data> get_data(const asset_descriptor& desc);

    private:
      void scan_dir(const std::filesystem::path& asset_dir);

    private:
      std::mutex m_cache_mutex;
      std::unordered_map<xg::Guid, asset_descriptor> m_cache_meta;
      std::unordered_map<xg::Guid, std::shared_ptr<asset_data>> m_cache_data;

      asset_loader m_loader;
      std::unique_ptr<details::fs_watcher> m_watcher;

    private:
      database() = default;

      static std::unique_ptr<database> m_instance;

      friend std::unique_ptr<database> std::make_unique<database>();
      friend details::fs_listener;
  };
}
