#include <filesystem>
#include <memory>

#include <SDL_image.h>

#include <glad/glad.h>
#include <GL/gl.h>

#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <fmt/format.h>

#include <trollworks-editor/app.hpp>
#include <trollworks-editor/project.hpp>
#include <trollworks-editor/events.hpp>

#include <trollworks-editor/ui/components/main-window.hpp>


namespace tw::editor {
  int app::run(int argc, char** argv) {
    project_manager::instance().parse_cli_args(argc, argv);

    int exit_code = 0;
    int sdlimg_flags = 0
      | IMG_INIT_PNG
      | IMG_INIT_JPG
      ;
    int win_flags = 0
      | SDL_WINDOW_OPENGL
      | SDL_WINDOW_SHOWN
      | SDL_WINDOW_MAXIMIZED
      | SDL_WINDOW_RESIZABLE
      ;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      fmt::print(stderr, "[SDL] Initialization failed: {}\n", SDL_GetError());
      exit_code = 1;
      goto err0;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    if ((IMG_Init(sdlimg_flags) & sdlimg_flags) == 0) {
      fmt::print(stderr, "[SDL_image] Initialization failed: {}\n", IMG_GetError());
      exit_code = 1;
      goto err1;
    }

    m_win = SDL_CreateWindow(
      "Trollworks Editor",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      win_flags
    );
    if (m_win == nullptr) {
      fmt::print(stderr, "[SDL] Could not create window: {}\n", SDL_GetError());
      exit_code = 1;
      goto err2;
    }

    m_glctx = SDL_GL_CreateContext(m_win);
    if (m_glctx == nullptr) {
      fmt::print(stderr, "[SDL] Could not create GL Context: {}\n", SDL_GetError());
      exit_code = 1;
      goto err3;
    }

    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
      fmt::print(stderr, "[GL] Could not load GL functions\n");
      exit_code = 1;
      goto err4;
    }

    m_uictx = ImGui::CreateContext();
    if (m_uictx == nullptr) {
      fmt::print(stderr, "[ImGui] Could not create ImGui context\n");
      exit_code = 1;
      goto err4;
    }

    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontDefault();
    ImGui::GetIO().ConfigFlags |= 0
      | ImGuiConfigFlags_DockingEnable
      | ImGuiConfigFlags_ViewportsEnable
      ;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL2_InitForOpenGL(m_win, m_glctx)) {
      fmt::print(stderr, "[ImGui] Could not initialize SDL2 backend\n");
      exit_code = 1;
      goto err5;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
      fmt::print(stderr, "[ImGui] Could not initialize OpenGL backend\n");
      exit_code = 1;
      goto err6;
    }

    events::instance().startup();
    main_loop();

    ImGui_ImplOpenGL3_Shutdown();

err6:
    ImGui_ImplSDL2_Shutdown();

err5:
    ImGui::DestroyContext(m_uictx);
    m_uictx = nullptr;

err4:
    SDL_GL_DeleteContext(m_glctx);
    m_glctx = nullptr;

err3:
    SDL_DestroyWindow(m_win);
    m_win = nullptr;

err2:
    IMG_Quit();

err1:
    SDL_Quit();

err0:
    return exit_code;
  }

  void app::main_loop() {
    auto ticks = SDL_GetTicks();

    while (m_cf == controlflow::loop) {
      auto frame_begin_ticks = SDL_GetTicks();
      auto delta_ticks       = frame_begin_ticks - ticks;
      ticks                  = frame_begin_ticks;
      auto delta_time        = delta_ticks / 1000.0f;

      process_events();
      update(delta_time);
    }
  }

  void app::process_events() {
    SDL_Event evt;
    SDL_WaitEvent(&evt);

    events::instance().process_event(&evt, m_cf);
    ImGui_ImplSDL2_ProcessEvent(&evt);

    switch (evt.type) {
      case SDL_QUIT:
        m_cf = controlflow::exit;
        break;

      case SDL_WINDOWEVENT:
        switch (evt.window.event) {
          case SDL_WINDOWEVENT_CLOSE:
            if (evt.window.windowID == SDL_GetWindowID(m_win)) {
              m_cf = controlflow::exit;
            }

            break;
        }

        break;
    }
  }

  void app::update(float delta_time) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    h.reset();
    h.render<ui::main_window>();

    ImGui::Render();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();

      SDL_GL_MakeCurrent(m_win, m_glctx);
    }

    SDL_GL_SwapWindow(m_win);
  }
}
