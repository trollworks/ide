#include <trollworks-editor/assets/type.hpp>

namespace tw::editor::assets {
  std::string asset_type_as_str(asset_type type) {
    switch (type) {
      case asset_type::directory:
        return "directory";

      case asset_type::texture:
        return "texture";

      case asset_type::audio:
        return "audio";

      case asset_type::video:
        return "video";

      case asset_type::script:
        return "script";

      case asset_type::object:
        return "object";

      case asset_type::raw:
      default:
        return "raw";
    }
  }
}
