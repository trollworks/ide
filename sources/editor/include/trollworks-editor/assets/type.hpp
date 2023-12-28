#pragma once

#include <string>

namespace tw::editor::assets {
  enum class asset_type {
    directory,
    texture,
    audio,
    video,
    script,
    object,
    raw
  };

  std::string asset_type_as_str(asset_type type);
}
