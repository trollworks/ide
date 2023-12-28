#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include <efsw/efsw.hpp>

namespace tw::editor::assets::details {
  using callback_type = std::function<void(const std::filesystem::path& )>;

  class fs_listener : public efsw::FileWatchListener {
    public:
      fs_listener(const std::filesystem::path& location, callback_type cb);

      void handleFileAction(
        efsw::WatchID wid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string old_filename
      ) override;

    private:
      std::filesystem::path m_location;
      callback_type m_cb;
  };

  class fs_watcher {
    public:
      fs_watcher(const std::filesystem::path& location, callback_type cb);
      ~fs_watcher();

    private:
      std::unique_ptr<fs_listener> m_listener;
      std::unique_ptr<efsw::FileWatcher> m_watcher;
      efsw::WatchID m_wid;
  };
}
