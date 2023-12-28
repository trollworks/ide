#include <imgui.h>

#include <trollworks-editor/data/components/transform.hpp>

namespace tw::editor::data {
  void transform_component::on_inspector() {
    float posv[] = {position.x, position.y};
    ImGui::InputFloat2("Position##inspector-cmp-transform-pos", posv);
    position.x = posv[0];
    position.y = posv[1];

    float scalev[] = {scale.x, scale.y};
    ImGui::InputFloat2("Scale##inspector-cmp-transform-scale", scalev);
    scale.x = scalev[0];
    scale.y = scalev[1];

    float anchorv[] = {anchor.x, anchor.y};
    ImGui::InputFloat2("Anchor##inspector-cmp-transform-anchor", anchorv);
    anchor.x = anchorv[0];
    anchor.y = anchorv[1];
  }
}
