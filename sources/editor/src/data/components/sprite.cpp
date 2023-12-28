#include <imgui.h>
#include <imgui/imgui_stdlib.h>

#include <trollworks-editor/data/components/sprite.hpp>
#include <trollworks-editor/ui/state/selection.hpp>
#include <trollworks-editor/assets/db.hpp>
#include <trollworks-editor/events.hpp>


namespace tw::editor::data {
  void sprite_component::on_inspector() {
    std::string path;

    if (texture_asset) {
      path = texture_asset->relpath().string();
    }

    ImGui::InputText(
      "Texture##inspector-cmp-sprite-tex",
      &path,
      ImGuiInputTextFlags_ReadOnly
    );

    if (ImGui::BeginDragDropTarget()) {
      auto payload = ImGui::AcceptDragDropPayload("dnd_asset_texture");
      if (payload != nullptr) {
        auto sel   = static_cast<const ui::asset_sel*>(payload->Data);
        auto asset = assets::database::instance().get_meta(sel->guid);

        if (asset && asset->type() == assets::asset_type::texture) {
          texture_asset = asset;
          events::instance().redraw();
        }
      }

      ImGui::EndDragDropTarget();
    }
  }
}
