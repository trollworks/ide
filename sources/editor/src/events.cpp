#include <imgui.h>

#include <trollworks-editor/events.hpp>
#include <trollworks-editor/project.hpp>

namespace tw::editor {
  std::unique_ptr<events> events::m_instance = nullptr;

  events& events::instance() {
    if (!m_instance) {
      m_instance = std::make_unique<events>();
    }

    return *m_instance;
  }

  void events::process_event(SDL_Event* evt, controlflow& cf) {
    if (ImGui::GetIO().WantSaveIniSettings) {
      project_manager::instance().save();
    }

    if (evt != nullptr && evt->type == m_base_evt_type) {
      switch (evt->user.code) {
        case STARTUP_EVENT_TYPE:
          project_manager::instance().on_startup();
          break;

        case QUIT_EVENT_TYPE:
          cf = controlflow::exit;
          break;

        case COMMAND_EVENT_TYPE: {
          auto boxed_fn = static_cast<command_type*>(evt->user.data1);
          auto fn = command_type(*boxed_fn);
          delete boxed_fn;

          fn(evt->user.data2);
          break;
        }

        case REDRAW_EVENT_TYPE:
        default:
          break;
      }
    }
  }

  void events::startup() const {
    send_event(STARTUP_EVENT_TYPE, nullptr, nullptr);
  }

  void events::quit() const {
    send_event(QUIT_EVENT_TYPE, nullptr, nullptr);
  }

  void events::redraw() const {
    send_event(REDRAW_EVENT_TYPE, nullptr, nullptr);
  }

  void events::command(command_type fn, void* data) const {
    auto boxed_fn = new command_type(fn);
    send_event(COMMAND_EVENT_TYPE, boxed_fn, data);
  }

  events::events() {
    m_base_evt_type = SDL_RegisterEvents(1);
  }

  void events::send_event(Uint32 type, void* data1, void* data2) const {
    if (m_base_evt_type != (Uint32)-1) {
      SDL_Event event;
      SDL_memset(&event, 0, sizeof(event));

      event.type       = m_base_evt_type;
      event.user.code  = type;
      event.user.data1 = data1;
      event.user.data2 = data2;

      SDL_PushEvent(&event);
    }
  }
}
