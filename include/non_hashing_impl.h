#pragma once

#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace rnd_cnt::impl {

template<class T, class Eq>
class non_hashing_impl {
public:
    non_hashing_impl() : rnd_(std::random_device()()) {}

    void add(T value) {
        if (contains(value)) return;
        elements_.push_back(std::move(value));
    }

    bool erase(const T& value) {
        auto value_pos = find_elem(value);
        if (value_pos == elements_.end()) return false;
        elements_.erase(value_pos);
        return true;
    }

    [[nodiscard]] T get_random_element() const {
        std::size_t random_index = rnd_() % elements_.size();
        return elements_[random_index];
    }

private:
    bool contains(const T& value) {
        return find_elem(value) != elements_.end();
    }

    decltype(auto) find_elem(const T& value) {
        return std::find_if(elements_.begin(), elements_.end(), [&value](auto v) -> bool {
            return Eq()(value, v);
        });
    }

    std::vector<T> elements_{};
    mutable std::mt19937 rnd_;
};

} // namespace rnd_cnt::impl
