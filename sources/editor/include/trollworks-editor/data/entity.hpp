#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <crossguid/guid.hpp>

#include <trollworks-editor/data/component.hpp>


namespace tw::editor::data {
  struct entity {
    xg::Guid guid;
    std::string name;
    std::unordered_map<std::string, std::unique_ptr<component>> components;
  };
}
