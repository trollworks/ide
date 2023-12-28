#include <unordered_set>

#include <imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <fmt/format.h>

#include <trollworks-editor/events.hpp>
#include <trollworks-editor/project.hpp>
#include <trollworks-editor/assets/db.hpp>

#include <trollworks-editor/ui/components/assets-view.hpp>
#include <trollworks-editor/ui/widgets/tree-view.hpp>


struct path_hasher {
  std::size_t operator()(const std::filesystem::path& p) const {
    return std::filesystem::hash_value(p);
  }
};

namespace tw::editor::ui {
  using asset_tree          = widgets::tree_view<assets::asset_descriptor>;
  using asset_tree_node     = asset_tree::tree_node;
  using gen_asset_tree_node = std::generator<asset_tree_node>;

  assets_view::assets_view(
    ImGuiID& dsid,
    bool& show,
    selection_type& sel_state
  ) : m_dsid(dsid),
      m_show(show),
      m_sel_state(sel_state)
  {}

  void assets_view::render(hooks& h) {
    auto& search = h.use_state<std::string>();

    if (!m_show) {
      return;
    }

    auto shown = ImGui::Begin("Assets##win-view-assets", &m_show);
    ImGui::SetWindowDock(ImGui::GetCurrentWindow(), m_dsid, ImGuiCond_FirstUseEver);

    if (shown) {
      auto size = ImGui::GetContentRegionAvail();

      ImGui::SetNextItemWidth(size.x);
      ImGui::InputTextWithHint(
        "##assets-search",
        "Search assets...",
        &search,
        ImGuiInputTextFlags_EscapeClearsAll
      );

      ImGui::Separator();

      auto asset_list = assets::database::instance().get_all_meta();
      if (!search.empty()) {
        auto included = std::unordered_set<std::filesystem::path, path_hasher>();

        for (const auto& asset : asset_list) {
          for (const auto& segment : asset.relpath()) {
            if (segment.string().find(search) != std::string::npos) {
              for (auto p = asset.relpath(); !p.empty(); p = p.parent_path()) {
                included.insert(p);
              }
            }
          }
        }

        auto filtered = asset_list
          | std::views::filter([&included](const assets::asset_descriptor& asset) {
              return included.count(asset.relpath());
            })
          ;

        asset_list = std::vector(filtered.begin(), filtered.end());
      }

      auto tree = asset_tree(
        "Assets",
        assets::asset_descriptor::empty(assets::asset_type::directory),
        asset_list
      );

      tree.walk([&](asset_tree_node node) -> gen_asset_tree_node {
        ImGui::PushID(node.data.guid().str().c_str());

        auto node_label = fmt::format("{}###asset-{}", node.name, node.data.guid().str());

        if (node.data.type() == assets::asset_type::directory) {
          auto node_flags = ImGuiTreeNodeFlags_SpanFullWidth;

          if (ImGui::TreeNodeEx(node_label.c_str(), node_flags)) {
            for (auto& child : node.children) {
              co_yield child;
            }

            ImGui::TreePop();
          }
        }
        else {
          auto is_selected = m_sel_state == asset_sel{.guid = node.data.guid()};
          auto node_flags = 0
            | ImGuiTreeNodeFlags_NoTreePushOnOpen
            | ImGuiTreeNodeFlags_Leaf
            | ImGuiTreeNodeFlags_SpanFullWidth
            | (is_selected ? ImGuiTreeNodeFlags_Selected : 0)
            ;

          if (ImGui::TreeNodeEx(node_label.c_str(), node_flags)) {
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
              m_sel_state = selection_type(asset_sel{.guid = node.data.guid()});
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
              auto dnd_type = fmt::format("dnd_asset_{}", assets::asset_type_as_str(node.data.type()));
              auto payload  = asset_sel{.guid = node.data.guid() };

              ImGui::SetDragDropPayload(dnd_type.c_str(), &payload, sizeof(asset_sel));
              ImGui::Text("%s", node.name.c_str());

              ImGui::EndDragDropSource();
            }
          }
        }

        ImGui::PopID();
      });
    }

    ImGui::End();
  }
}
