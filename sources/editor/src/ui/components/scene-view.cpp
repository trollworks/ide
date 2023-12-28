#include <imgui.h>
#include <imgui/imgui_internal.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

#include <trollworks-editor/ui/components/scene-view.hpp>


namespace tw::editor::ui {
  namespace details {
    class camera {
      public:
        void update(ImVec2 size);

        glm::vec2 pos() const;
        float zoom() const;

      private:
        glm::vec2 m_pos{0, 0};
        float m_zoom{1.0f};

        bool m_is_dragging{false};
        glm::vec2 m_last_mouse_pos{0, 0};
    };

    class render_target {
      public:
        void update(ImVec2 size);
        void render(ImVec2 size, const camera& cam);

        ImTextureID texture() const;

      private:
        bool m_first_frame{true};
        GLuint m_tex_id;
        GLuint m_fb_id;
        ImVec2 m_prev_size;
    };
  }

  scene_view::scene_view(
    ImGuiID& dsid,
    bool& show,
    selection_type& sel_state
  ) : m_dsid(dsid),
      m_show(show),
      m_sel_state(sel_state)
  {}

  void scene_view::render(hooks& h) {
    auto& camera        = h.use_state<details::camera>();
    auto& render_target = h.use_state<details::render_target>();

    if (!m_show) {
      return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    auto shown = ImGui::Begin(
      "Scene##win-view-scene",
      &m_show,
      ImGuiWindowFlags_MenuBar
    );
    ImGui::PopStyleVar();

    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      auto size = ImGui::GetContentRegionAvail();

      camera.update(size);
      render_target.update(size);
      render_target.render(size, camera);

      if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Run")) {
          ;
        }

        ImGui::EndMenuBar();
      }

      ImGui::Image(render_target.texture(), size, {0, 1}, {1, 0});
    }

    ImGui::End();
  }

  namespace details {
    void camera::update(ImVec2 size) {
      // Zoom
      auto zoom_amount = ImGui::GetIO().MouseWheel;

      if (ImGui::IsWindowHovered() && zoom_amount != 0) {
        auto mouse_pos  = ImGui::GetMousePos();
        auto window_pos = ImGui::GetWindowPos();

        auto rel_mouse_pos = glm::vec2(
          mouse_pos.x - window_pos.x,
          mouse_pos.y - window_pos.y
        );
        auto sz = glm::vec2(size.x, size.y);
        auto prev_world_mouse_pos = (rel_mouse_pos - sz * 0.5f) / m_zoom;

        auto zoom_factor = 1.1f;
        m_zoom *= zoom_amount > 0 ? zoom_factor : (1.0f / zoom_factor);

        auto next_world_mouse_pos = (rel_mouse_pos - sz * 0.5f) / m_zoom;

        auto delta = prev_world_mouse_pos - next_world_mouse_pos;
        m_pos += delta;
      }

      // Drag
      if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
        if (!m_is_dragging) {
          auto mouse_pos   = ImGui::GetMousePos();
          m_last_mouse_pos = {mouse_pos.x, mouse_pos.y};
          m_is_dragging    = true;
        }
      }
      else {
        m_is_dragging = false;
      }

      if (m_is_dragging) {
        auto mouse_pos = ImGui::GetMousePos();
        auto delta_pos = glm::vec2(
          m_last_mouse_pos.x - mouse_pos.x,
          mouse_pos.y - m_last_mouse_pos.y
        );

        auto movement = delta_pos;// * CAMERA_DRAG_SCALE;
        m_pos += movement;

        m_last_mouse_pos = {mouse_pos.x, mouse_pos.y};
      }
    }

    glm::vec2 camera::pos() const {
      return m_pos;
    }

    float camera::zoom() const {
      return m_zoom;
    }

    void render_target::update(ImVec2 size) {
      if (size.x != m_prev_size.x || size.y != m_prev_size.y) {
        if (!m_first_frame) {
          glDeleteTextures(1, &m_tex_id);
          glDeleteFramebuffers(1, &m_fb_id);
        }

        glGenTextures(1, &m_tex_id);
        glBindTexture(GL_TEXTURE_2D, m_tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenFramebuffers(1, &m_fb_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fb_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_prev_size   = size;
        m_first_frame = false;
      }
    }

    void render_target::render(ImVec2 size, const camera& cam) {
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      glBindFramebuffer(GL_FRAMEBUFFER, m_fb_id);
      glViewport(0, 0, size.x, size.y);

      auto cam_pos       = cam.pos();
      auto cam_zoom      = cam.zoom();
      auto cam_transform = glm::ortho(
        cam_pos.x - size.x / (2 * cam_zoom),
        cam_pos.x + size.x / (2 * cam_zoom),
        cam_pos.y - size.y / (2 * cam_zoom),
        cam_pos.y + size.y / (2 * cam_zoom)
      );

      glClearColor(0.2f, 0.2f, 0.2f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::vec4 vertices[4] = {
        {10, 10, 0, 1},
        {15, 10, 0, 1},
        {15, 15, 0, 1},
        {10, 15, 0, 1}
      };

      for (auto i = 0; i < 4; i++) {
        vertices[i] = cam_transform * vertices[i];
      }

      glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        for (int i = 0; i < 4; i++) {
          glVertex2f(vertices[i].x, vertices[i].y);
        }
      glEnd();

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }

    ImTextureID render_target::texture() const {
      return (ImTextureID) (uintptr_t) m_tex_id;
    }
  }
}
