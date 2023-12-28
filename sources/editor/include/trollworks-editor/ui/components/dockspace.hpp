#pragma once

#include <trollworks-editor/ui/hooks.hpp>

#include <trollworks-editor/ui/state/dockspace.hpp>

namespace tw::editor::ui {
  class dockspace {
    public:
      dockspace(dockspace_state& state);

      void render(hooks& h);

    private:
      dockspace_state& m_state;
  };
}
