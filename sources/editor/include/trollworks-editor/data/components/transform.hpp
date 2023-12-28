#pragma once

#include <glm/vec2.hpp>

#include <trollworks-editor/data/component.hpp>


namespace tw::editor::data {
  struct transform_component : public component {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    glm::vec2 anchor{0.5f, 0.5f};

    virtual void on_inspector();
  };
}
