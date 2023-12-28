#pragma once

#include <optional>
#include <memory>

#include <glm/vec2.hpp>

#include <trollworks-editor/data/component.hpp>
#include <trollworks-editor/assets/descriptor.hpp>
#include <trollworks-editor/assets/data/texture.hpp>


namespace tw::editor::data {
  struct sprite_component : public component {
    std::optional<assets::asset_descriptor> texture_asset;

    virtual void on_inspector();
  };
}
