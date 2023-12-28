#pragma once

namespace tw::editor::ui {
  struct view_state {
    bool show_entities{true};
    bool show_systems{true};
    bool show_assets{true};
    bool show_scene{true};
    bool show_inspector{true};
    bool show_console{true};
  };
}
