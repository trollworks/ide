#pragma once

#include <imgui.h>

#include <trollworks-editor/ui/hooks.hpp>
#include <trollworks-editor/ui/state/selection.hpp>


namespace tw::editor::ui {
  class systems_view {
    public:
      systems_view(ImGuiID& dsid, bool& show, selection_type& sel_state);

      void render(hooks& h);

    private:
      ImGuiID& m_dsid;
      bool& m_show;
      selection_type& m_sel_state;
  };
}
