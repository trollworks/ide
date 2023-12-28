#pragma once

#include <imgui.h>

#include <trollworks-editor/ui/hooks.hpp>
#include <trollworks-editor/ui/state/selection.hpp>


namespace tw::editor::ui {
  class inspector_view {
    public:
      inspector_view(ImGuiID& dsid, bool& show, selection_type& sel_state);

      void render(hooks& h);

    private:
      void show_selection(empty_sel& s);
      void show_selection(entity_sel& s);
      void show_selection(system_sel& s);
      void show_selection(asset_sel& s);

    private:
      ImGuiID& m_dsid;
      bool& m_show;
      selection_type& m_sel_state;
  };
}
