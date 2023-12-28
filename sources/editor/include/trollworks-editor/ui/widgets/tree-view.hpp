#pragma once

#include <filesystem>
#include <concepts>
#include <string>

#include <generator.hpp>


namespace tw::editor::ui::widgets {
  template <typename T>
  concept has_path = requires(const T& obj) {
    { obj.relpath() } -> std::same_as<const std::filesystem::path& >;
  };

  template <has_path T>
  class tree_view {
    public:
      struct tree_node {
        std::string name;
        std::vector<tree_node> children;
        T data;
      };

      using walk_cb = std::function<std::generator<tree_node>(tree_node)>;

    public:
      tree_view(
        const std::string& root_label,
        const T& root_data,
        std::vector<T> items
      ) : root(tree_node{.name = root_label, .data = root_data})
      {
        for (const auto& item : items) {
          auto current_node = &root;

          for (const auto& segment : item.relpath()) {
            bool found = false;

            for (auto& child : current_node->children) {
              if (child.name == segment) {
                current_node = &child;
                found = true;
                break;
              }
            }

            if (!found) {
              current_node->children.emplace_back(tree_node{
                .name = segment.string(),
                .data = item
              });
              current_node = &current_node->children.back();
            }
          }

          current_node->data = item;
        }
      }

      void walk(walk_cb fn) const {
        walk_impl(root, fn);
      }

    private:
      void walk_impl(const tree_node& node, walk_cb fn) const {
        for (const auto& child : fn(node)) {
          walk_impl(child, fn);
        }
      }

    private:
      tree_node root;
  };
}
