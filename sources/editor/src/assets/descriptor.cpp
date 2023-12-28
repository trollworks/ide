#include <fstream>

#include <trollworks-editor/assets/descriptor.hpp>

using namespace nlohmann;

namespace tw::editor::assets {
  static asset_type get_asset_type(const std::filesystem::path& asset_path);

  xg::Guid asset_descriptor::guid() const {
    return m_guid;
  }

  const std::filesystem::path& asset_descriptor::fullpath() const {
    return m_fullpath;
  }

  const std::filesystem::path& asset_descriptor::relpath() const {
    return m_relpath;
  }

  nlohmann::json asset_descriptor::attributes() const {
    return m_attrs;
  }

  asset_type asset_descriptor::type() const {
    return m_type;
  }

  void asset_descriptor::save() const {
    auto metapath = m_fullpath;
    metapath += ".meta";

    auto f = std::ofstream(metapath);
    auto metadata = json{
      {"guid", m_guid},
      {"attrs", m_attrs}
    };
    f << metadata.dump(2) << std::endl;
  }

  void asset_descriptor::erase() const {
    auto metapath = m_fullpath;
    metapath += ".meta";

    if (std::filesystem::exists(metapath)) {
      std::filesystem::remove(metapath);
    }
  }

  asset_descriptor asset_descriptor::empty(asset_type type) {
    auto desc   = asset_descriptor();
    desc.m_guid = xg::Guid();
    desc.m_type = type;
    return desc;
  }

  asset_descriptor asset_descriptor::from_path(
    const std::filesystem::path& asset_dir,
    const std::filesystem::path& relpath
  ) {
    auto fullpath = asset_dir / relpath;
    auto metapath = fullpath;
    metapath += ".meta";

    if (!std::filesystem::exists(metapath)) {
      auto entry = asset_descriptor{};

      entry.m_guid     = xg::newGuid();
      entry.m_fullpath = fullpath;
      entry.m_relpath  = relpath;
      entry.m_attrs    = json{};
      entry.m_type     = get_asset_type(fullpath);

      entry.save();
      return entry;
    }
    else {
      auto f = std::ifstream(metapath);
      auto metadata = json::parse(f);

      auto entry = asset_descriptor{};

      entry.m_guid     = xg::Guid(metadata.at("guid").get<std::string>());
      entry.m_fullpath = fullpath;
      entry.m_relpath  = relpath;
      entry.m_attrs    = (metadata.contains("attrs") ? metadata.at("attrs") : json{});
      entry.m_type     = get_asset_type(fullpath);

      return entry;
    }
  }

  static asset_type get_asset_type(const std::filesystem::path& asset_path) {
    if (std::filesystem::is_directory(asset_path)) {
      return asset_type::directory;
    }

    std::unordered_map<std::string, asset_type> types = {
      {std::string(".bmp"),   asset_type::texture},
      {std::string(".gif"),   asset_type::texture},
      {std::string(".png"),   asset_type::texture},
      {std::string(".jpg"),   asset_type::texture},
      {std::string(".jpeg"),  asset_type::texture},
      {std::string(".wav"),   asset_type::audio},
      {std::string(".midi"),  asset_type::audio},
      {std::string(".mp3"),   asset_type::audio},
      {std::string(".ogg"),   asset_type::audio},
      {std::string(".avi"),   asset_type::video},
      {std::string(".ogv"),   asset_type::video},
      {std::string(".lua"),   asset_type::script},
      {std::string(".asset"), asset_type::object}
    };

    if (
      auto pair = types.find(asset_path.extension().string());
      pair != types.end()
    ) {
      return pair->second;
    }

    return asset_type::raw;
  }
}
