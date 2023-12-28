#include <utility>

#include <trollworks-editor/ui/state/selection.hpp>

namespace tw::editor::ui {
  bool operator==(const selection_type& a, const empty_sel& b) {
    return std::holds_alternative<empty_sel>(a);
  }

  bool operator==(const selection_type& a, const entity_sel& b) {
    if (std::holds_alternative<entity_sel>(a)) {
      auto alt = std::get<entity_sel>(a);
      return alt.guid == b.guid;
    }

    return false;
  }

  bool operator==(const selection_type& a, const system_sel& b) {
    if (std::holds_alternative<system_sel>(a)) {
      auto alt = std::get<system_sel>(a);
      return alt.id == b.id;
    }

    return false;
  }

  bool operator==(const selection_type& a, const asset_sel& b) {
    if (std::holds_alternative<asset_sel>(a)) {
      auto alt = std::get<asset_sel>(a);
      return alt.guid == b.guid;
    }

    return false;
  }
}
