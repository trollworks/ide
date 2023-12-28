#include <ranges>

#include <trollworks-editor/assets/db.hpp>

namespace tw::editor::assets {
  std::unique_ptr<database> database::m_instance = nullptr;

  database& database::instance() {
    if (!m_instance) {
      m_instance = std::make_unique<database>();
    }

    return *m_instance;
  }

  void database::set_location(const std::filesystem::path& asset_dir) {
    scan_dir(asset_dir);

    m_watcher = std::make_unique<details::fs_watcher>(
      asset_dir,
      [&](auto location) { scan_dir(location); }
    );
  }

  std::optional<asset_descriptor> database::get_meta(xg::Guid guid) {
    auto guard = std::lock_guard<std::mutex>(m_cache_mutex);

    if (m_cache_meta.contains(guid)) {
      return m_cache_meta.at(guid);
    }

    return std::nullopt;
  }

  std::vector<asset_descriptor> database::get_all_meta() {
    auto guard = std::lock_guard<std::mutex>(m_cache_mutex);
    auto assets = m_cache_meta | std::views::values;
    return std::vector(assets.begin(), assets.end());
  }

  std::shared_ptr<asset_data> database::get_data(const asset_descriptor& desc) {
    auto guard = std::lock_guard<std::mutex>(m_cache_mutex);

    if (!m_cache_data.contains(desc.guid())) {
      auto data = m_loader.load(desc);
      if (!data) {
        return nullptr;
      }

      m_cache_data.insert({desc.guid(), data});
      return data;
    }

    return m_cache_data.at(desc.guid());
  }

  void database::scan_dir(const std::filesystem::path& asset_dir) {
    auto guard = std::lock_guard<std::mutex>(m_cache_mutex);

    m_cache_meta.clear();
    m_cache_data.clear();

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(asset_dir)) {
      if (dir_entry.path().extension() != ".meta") {
        auto relpath = std::filesystem::relative(dir_entry.path(), asset_dir);
        auto asset = asset_descriptor::from_path(asset_dir, relpath);
        m_cache_meta.insert({asset.guid(), asset});
      }
    }
  }
}
