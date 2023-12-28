#include <chrono>
#include <ctime>

#include <fmt/format.h>

#include <trollworks-editor/logger.hpp>
#include <trollworks-editor/events.hpp>


namespace tw::editor {
  static std::string loglevel_as_str(loglevel lvl) {
    switch (lvl) {
      case loglevel::debug:
        return "DEBUG";

      case loglevel::info:
        return "INFO";

      case loglevel::warning:
        return "WARNING";

      case loglevel::error:
      default:
        return "ERROR";
    }
  }

  std::unique_ptr<logger> logger::m_instance = nullptr;

  logger& logger::instance() {
    if (!m_instance) {
      m_instance = std::make_unique<logger>();
    }

    return *m_instance;
  }

  void logger::log(loglevel lvl, const std::string& msg) {
    auto now     = std::chrono::system_clock::now();
    auto ts      = std::chrono::system_clock::to_time_t(now);
    auto localts = std::localtime(&ts);

    auto hh = localts->tm_hour;
    auto mm = localts->tm_min;
    auto ss = localts->tm_sec;

    auto content = fmt::format(
      "[{:02}:{:02}:{:02}] {}: {}\n",
      hh, mm, ss,
      loglevel_as_str(lvl),
      msg
    );

    m_buffer.append(content.c_str());
    events::instance().redraw();
  }

  void logger::clear() {
    m_buffer.clear();
  }

  const char* logger::begin() const {
    return m_buffer.begin();
  }

  const char* logger::end() const {
    return m_buffer.end();
  }
}
