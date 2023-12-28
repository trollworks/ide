#pragma once

#include <concepts>

#include <entt/entt.hpp>


namespace tw::editor::ui {
  class hooks;

  template <typename T>
  concept drawable = requires(T& d, hooks& h) {
    { d.render(h) } -> std::same_as<void>;
  };

  class hooks {
    public:
      void reset() {
        m_current_index = 0;
      }

      template <typename T>
      T& use_state() {
        return use_state(T());
      }

      template <typename T>
      T& use_state(T initial_val) {
        entt::id_type key = m_current_index++;

        if (!m_state.contains<T>(key)) {
          m_state.emplace_as<T>(key, initial_val);
        }

        return m_state.get<T>(key);
      }

      template <drawable D, typename... Args>
      void render(Args&&... args) {
        auto d = D(std::forward<Args>(args)...);
        d.render(*this);
      }

    private:
      entt::id_type m_current_index{0};
      entt::registry::context m_state;
  };
}
