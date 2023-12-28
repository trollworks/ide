#include <imgui.h>

#include <trollworks-editor/assets/data/texture.hpp>


namespace tw::editor::assets {
  texture_data::texture_data(const std::filesystem::path& path) {
    auto surface = IMG_Load(path.string().c_str());
    if (surface == nullptr) {
      throw std::runtime_error(IMG_GetError());
    }

    m_width  = surface->w;
    m_height = surface->h;

    glGenTextures(1, &m_tex_id);
    glBindTexture(GL_TEXTURE_2D, m_tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
  }

  texture_data::~texture_data() {
    glDeleteTextures(1, &m_tex_id);
  }

  void texture_data::on_inspector() const {
    ImGui::SeparatorText("Texture");

    int size[] = {m_width, m_height};
    ImGui::InputInt2("Size", size, ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Preview:");

    auto region = ImGui::GetContentRegionAvail();
    auto preview_size = ImVec2(
      region.x,
      (region.x * m_height) / m_width
    );
    ImGui::Image(reinterpret_cast<ImTextureID>(m_tex_id), preview_size);
  }
}
