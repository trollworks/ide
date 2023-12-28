#pragma once

#include <optional>
#include <variant>

#include <crossguid/guid.hpp>


namespace tw::editor::ui {
  struct empty_sel {};

  struct entity_sel {
    xg::Guid guid;
  };

  struct system_sel {
    size_t id;
  };

  struct asset_sel {
    xg::Guid guid;
  };

  using selection_type = std::variant<
    empty_sel,
    entity_sel,
    system_sel,
    asset_sel
  >;

  bool operator==(const selection_type& a, const empty_sel& b);
  bool operator==(const selection_type& a, const entity_sel& b);
  bool operator==(const selection_type& a, const system_sel& b);
  bool operator==(const selection_type& a, const asset_sel& b);
}
