#pragma once

#include <filesystem>

#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

#include <trollworks-editor/assets/data.hpp>


namespace tw::editor::assets {
  class texture_data final : public asset_data {
    public:
      texture_data(const std::filesystem::path& path);
      ~texture_data();

      virtual void on_inspector() const;

    private:
      GLuint m_tex_id;
      int m_width;
      int m_height;
  };
}
