#pragma once


namespace tw::editor::assets {
  class asset_data {
    public:
      virtual void on_inspector() const = 0;
  };
}
