#pragma once

#include <functional>
#include <memory>
#include <string>

#include <SDL.h>

#include <trollworks-editor/controlflow.hpp>

#define STARTUP_EVENT_TYPE    0
#define QUIT_EVENT_TYPE       1
#define REDRAW_EVENT_TYPE     2
#define COMMAND_EVENT_TYPE    3


namespace tw::editor {
  using command_type = std::function<void(void*)>;

  class events {
    public:
      static events& instance();

    public:
      void process_event(SDL_Event* evt, controlflow& cf);

      void startup() const;
      void quit() const;
      void redraw() const;
      void command(command_type fn, void* data) const;

    private:
      events();

      void send_event(Uint32 type, void* data1, void* data2) const;

    private:
      Uint32 m_base_evt_type;

    private:
      static std::unique_ptr<events> m_instance;

      friend std::unique_ptr<events> std::make_unique<events>();
  };
}
