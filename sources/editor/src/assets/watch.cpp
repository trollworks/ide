#include <trollworks-editor/assets/db.hpp>
#include <trollworks-editor/assets/watch.hpp>
#include <trollworks-editor/events.hpp>


namespace tw::editor::assets::details {
  fs_listener::fs_listener(const std::filesystem::path& location, callback_type cb)
    : m_location(location), m_cb(cb) {}

  void fs_listener::handleFileAction(
    efsw::WatchID wid,
    const std::string& dir,
    const std::string& filename,
    efsw::Action action,
    std::string old_filename
  ) {
    if (filename.ends_with(".meta")) {
      return;
    }

    auto dirpath      = std::filesystem::path(dir);
    auto filepath     = std::filesystem::path(filename);
    auto old_filepath = std::filesystem::path(old_filename);

    switch (action) {
      case efsw::Actions::Delete: {
        auto entry = asset_descriptor::from_path(dirpath, filepath);
        entry.erase();
        break;
      }

      case efsw::Actions::Moved: {
        auto old_entry = asset_descriptor::from_path(dirpath, old_filepath);
        old_entry.erase();
        break;
      }

      default:
        break;
    }

    m_cb(m_location);

    events::instance().redraw();
  }

  fs_watcher::fs_watcher(const std::filesystem::path& location, callback_type cb) {
    m_listener = std::make_unique<fs_listener>(location, cb);
    m_watcher  = std::make_unique<efsw::FileWatcher>();
    m_wid      = m_watcher->addWatch(location.string(), m_listener.get(), true);
    m_watcher->watch();
  }

  fs_watcher::~fs_watcher() {
    m_watcher->removeWatch(m_wid);
  }
}
