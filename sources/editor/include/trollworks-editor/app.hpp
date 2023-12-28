#pragma once

#include <SDL.h>
#include <imgui.h>
#include <efsw/efsw.hpp>

#include <trollworks-editor/controlflow.hpp>
#include <trollworks-editor/ui/hooks.hpp>


namespace tw::editor {
  class app {
    public:
      int run(int argc, char** argv);

    private:
      void main_loop();
      void process_events();
      void update(float delta_time);

    private:
      efsw::FileWatcher m_file_watcher;

      controlflow m_cf{controlflow::loop};
      SDL_Window* m_win{nullptr};
      SDL_GLContext m_glctx{nullptr};
      ImGuiContext* m_uictx{nullptr};

      ui::hooks h;
  };
}
