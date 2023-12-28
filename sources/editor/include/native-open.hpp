#pragma once

#include <filesystem>

#include <fmt/format.h>

#if defined(_WIN32) || defined(_WIN64)
# include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
# include <cstdlib>
#else
  static_assert(false, "Unsupported platform.");
#endif

#if defined(__linux__)
# define NATIVEOPEN_BINARY    "xdg-open"
#elif defined(__APPLE__)
# define NATIVEOPEN_BINARY    "open"
#endif

namespace nativeopen {
  inline void open_file(const std::filesystem::path& filename) {
#if defined(_WIN32) || defined(_WIN64)
    auto instance = ShellExecuteA(
      nullptr,
      "open",
      filename.string().c_str(),
      nullptr,
      nullptr,
      SW_SHOWNORMAL
    );

    if (reinterpret_cast<intptr_t>(instance) <= 32) {
      throw std::runtime_error("Could not open file");
    }
#else
    auto command = fmt::format("{} {} > /dev/null 2>&1", NATIVEOPEN_BINARY, filename.string());
    auto result  = system(command.c_str());

    if (result != 0) {
      throw std::runtime_error("Could not open file");
    }
#endif
  }
}
