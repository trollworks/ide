#pragma once

#include <functional>
#include <memory>

namespace tw::editor::data {
  struct component {
    virtual void on_inspector() = 0;
  };

  using component_factory = std::function<std::unique_ptr<component>()>;
}
