#pragma once

#include <vector>
#include <memory>

#include <ordered-map.hpp>
#include <crossguid/guid.hpp>

#include <trollworks-editor/data/entity.hpp>
#include <trollworks-editor/data/system.hpp>


namespace tw::editor::data {
  struct scene {
    ordered_map<xg::Guid, std::shared_ptr<entity>> entities;
    std::vector<std::unique_ptr<system>> systems;
  };
}
