#pragma once

#include <memory>

#include <trollworks-editor/assets/descriptor.hpp>
#include <trollworks-editor/assets/data.hpp>

namespace tw::editor::assets {
  struct asset_loader {
    std::shared_ptr<asset_data> load(const asset_descriptor& desc) const;
  };
}
