#pragma once

#include <vector>
#include <utility>


template <typename K, typename V>
class ordered_map {
  public:
    using value_type     = std::pair<K, V>;
    using container_type = std::vector<value_type>;

    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    using reverse_iterator       = container_type::reverse_iterator;
    using const_reverse_iterator = container_type::const_reverse_iterator;

    using size_type = container_type::size_type;

  public:
    ordered_map() {}
    ordered_map(const ordered_map& other) {
      m_data.reserve(other.m_data.size());

      for (auto pair : other.m_data) {
        m_data.push_back(pair);
      }
    }

    ordered_map(ordered_map&& other) {
      m_data = std::move(other.m_data);
      other.m_data = container_type();
    }

    ordered_map(std::initializer_list<value_type> init) {
      m_data.reserve(init.size());

      for (auto pair : init) {
        m_data.push_back(pair);
      }
    }

    ordered_map& operator=(const ordered_map& other) {
      m_data.clear();
      m_data.reserve(other.m_data.size());

      for (auto pair : other.m_data) {
        m_data.push_back(pair);
      }

      return *this;
    }

    ordered_map& operator=(ordered_map&& other) noexcept {
      m_data = std::move(other.m_data);
      other.m_data = container_type();
      return *this;
    }

    ordered_map& operator=(std::initializer_list<value_type> init) {
      m_data.reserve(init.size());

      for (auto pair : init) {
        m_data.push_back(pair);
      }

      return *this;
    }

    V& at(const K& key) {
      for (auto& [k, v] : m_data) {
        if (k == key) {
          return v;
        }
      }

      throw std::out_of_range("key not found");
    }

    const V& at(const K& key) const {
      for (const auto& [k, v] : m_data) {
        if (k == key) {
          return v;
        }
      }

      throw std::out_of_range("key not found");
    }

    V& operator[](const K& key) {
      for (auto& [k, v] : m_data) {
        if (k == key) {
          return v;
        }
      }

      m_data.push_back(value_type(key, V()));
      return m_data.back();
    }

    V& operator[](K&& key) {
      for (auto& [k, v] : m_data) {
        if (k == key) {
          return v;
        }
      }

      m_data.push_back(value_type(key, V()));
      return m_data.back();
    }

    iterator begin() noexcept {
      return m_data.begin();
    }

    const_iterator begin() const noexcept {
      return m_data.begin();
    }

    const_iterator cbegin() const noexcept {
      return m_data.cbegin();
    }

    iterator end() noexcept {
      return m_data.end();
    }

    const_iterator end() const noexcept {
      return m_data.end();
    }

    const_iterator cend() const noexcept {
      return m_data.cend();
    }

    reverse_iterator rbegin() noexcept {
      return m_data.rbegin();
    }

    const_reverse_iterator rbegin() const noexcept {
      return m_data.rbegin();
    }

    const_reverse_iterator crbegin() const noexcept {
      return m_data.crbegin();
    }

    reverse_iterator rend() noexcept {
      return m_data.rend();
    }

    const_reverse_iterator rend() const noexcept {
      return m_data.rend();
    }

    const_reverse_iterator crend() const noexcept {
      return m_data.crend();
    }

    bool empty() const noexcept {
      return m_data.empty();
    }

    size_type size() const noexcept {
      return m_data.size();
    }

    size_type max_size() const noexcept {
      return m_data.max_size();
    }

    void clear() noexcept {
      m_data.clear();
    }

    std::pair<iterator, bool> insert(const value_type& value) {
      for (auto it = begin(); it != end(); it++) {
        if (it->first == value.first) {
          return {it, false};
        }
      }

      m_data.push_back(value);
      auto new_it = end();
      new_it--;
      return {new_it, true};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
      for (auto it = begin(); it != end(); it++) {
        if (it->first == value.first) {
          return {it, false};
        }
      }

      m_data.push_back(value);
      auto new_it = end();
      new_it--;
      return {new_it, true};
    }

    void insert(std::initializer_list<value_type> ilist) {
      for (auto pair : ilist) {
        insert(pair);
      }
    }

    size_type erase(const K& key) {
      auto new_data = container_type();
      new_data.reserve(m_data.size());

      for (auto& [k, v] : m_data) {
        if (k != key) {
          new_data.push_back(value_type(k, v));
        }
      }

      auto erased_count = m_data.size() - new_data.size();
      m_data = new_data;

      return erased_count;
    }

    bool contains(const K& key) const {
      for (auto& [k, _] : m_data) {
        if (k == key) {
          return true;
        }
      }

      return false;
    }

  private:
    container_type m_data;
};
