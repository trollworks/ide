#pragma once

#include <imgui.h>

#include <trollworks-editor/ui/hooks.hpp>


namespace tw::editor::ui {
  class console_view {
    public:
      console_view(ImGuiID& dsid, bool& show);

      void render(hooks& h);

    private:
      ImGuiID& m_dsid;
      bool& m_show;
  };
}
