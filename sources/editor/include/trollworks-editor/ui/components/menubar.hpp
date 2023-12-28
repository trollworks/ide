#pragma once

#include <trollworks-editor/ui/hooks.hpp>

#include <trollworks-editor/ui/state/view.hpp>

namespace tw::editor::ui {
  class menubar {
    public:
      menubar(view_state& state);

      void render(hooks& h);

    private:
      view_state& m_state;
  };
}
