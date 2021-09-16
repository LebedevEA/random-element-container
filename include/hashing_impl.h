#pragma once

#include <random>
#include <vector>
#include <unordered_map>

namespace rnd_cnt::impl {

template<class T, class Hash, class Eq>
class hashing_impl {
public:
    hashing_impl() : rnd_(std::random_device()()) {}

    void add(T value) {
        if (element_to_index_.count(value) > 0) return;
        elements_.push_back(std::move(value));
        std::size_t index = elements_.size() - 1;
        try {
            element_to_index_[elements_.back()] = index;
        } catch (...) {
            elements_.pop_back();
            throw;
        }
    }

    bool erase(const T &value) {
        auto value_pos = element_to_index_.find(value);
        if (value_pos == element_to_index_.end()) return false;

        std::size_t value_index = value_pos->second;
        auto back_pos = element_to_index_.find(elements_.back());
        elements_[value_index] = std::move(elements_.back());

        back_pos->second = value_index;
        elements_.pop_back();
        element_to_index_.erase(value_pos);

        return true;
    }

    [[nodiscard]] T &get_random_element() {
        std::size_t random_index = rnd_() % elements_.size();
        return elements_[random_index];
    }

    [[nodiscard]] const T &get_random_element() const {
        std::size_t random_index = rnd_() % elements_.size();
        return elements_[random_index];
    }

private:
    std::vector<T> elements_{};
    std::unordered_map<T, std::size_t, Hash, Eq> element_to_index_{};
    mutable std::mt19937 rnd_;
};

} // namespace rnd_cnt::impl
