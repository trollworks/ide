#include <trollworks-editor/assets/loader.hpp>

#include <trollworks-editor/assets/data/texture.hpp>
#include <trollworks-editor/assets/data/raw.hpp>


namespace tw::editor::assets {
  std::shared_ptr<asset_data> asset_loader::load(const asset_descriptor& desc) const {
    std::shared_ptr<asset_data> data = nullptr;

    switch (desc.type()) {
      case asset_type::texture:
        data = std::make_shared<texture_data>(desc.fullpath());
        break;

      case asset_type::raw:
        data = std::make_shared<raw_data>(desc.fullpath());
        break;

      case asset_type::directory:
        throw std::runtime_error("cannot load directory assets");

      default:
        throw std::runtime_error("not implemented");
    }

    return data;
  }
}
