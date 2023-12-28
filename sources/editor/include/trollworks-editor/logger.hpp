#pragma once

#include <memory>
#include <string>

#include <imgui.h>


namespace tw::editor {
  enum class loglevel {
    debug,
    info,
    warning,
    error
  };

  class logger {
    public:
      static logger& instance();

    public:
      void log(loglevel lvl, const std::string& msg);

      void clear();

      const char* begin() const;
      const char* end() const;

    private:
      logger() = default;

    private:
      ImGuiTextBuffer m_buffer;

      static std::unique_ptr<logger> m_instance;

      friend std::unique_ptr<logger> std::make_unique<logger>();
  };
}
