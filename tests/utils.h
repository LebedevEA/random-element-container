#pragma once

#include <unordered_set>
#include "random_container.h"
#include "doctest.h"

using namespace rnd_cnt;

namespace correctness {

namespace impl {

template<class T, class Eq, class V>
decltype(auto) find_elem(const V& values, const T& value) {
    return std::find_if(values.begin(), values.end(), [&value](auto v) -> bool {
        return Eq()(value, v);
    });
}

template<class T, class Eq, class V>
bool contains(const V& values, const T& value) {
    return find_elem<T, Eq>(values, value) != values.end();
}

} // namespace test_utils_impl

template<class T, class Eq, class Hash, class V>
void test_gives_only(const random_container<T, Hash, Eq>& cont, const V& acceptable, int attempts = 1000) {
    for (int i = 0; i < attempts; i++) {
        auto elem = cont.get_random_element();
        CHECK(impl::contains<T, Eq>(acceptable, elem));
    }
}

struct std_hashable {
    int x;

    explicit operator int() const {
        return x;
    }

    bool operator==(const std_hashable& other) const {
        return this->x == other.x;
    }
};

struct not_hashable {
    int x;

    explicit operator int() const {
        return x;
    }

    bool operator==(const not_hashable& other) const noexcept {
        return this->x == other.x;
    }
};

struct not_hashable_hash {
    std::size_t operator()(const not_hashable& h) const noexcept {
        return std::hash<int>()(static_cast<int>(h));
    }
};

namespace impl {

template<class T>
void vec_of_impl(std::vector<T>& vec) {}

template<class T, class U, class... Args>
void vec_of_impl(std::vector<T>& vec, const U& value, const Args& ... values) {
    vec.push_back({value});
    vec_of_impl(vec, values...);
}

}

template<class T, class... Args>
std::vector<T> vec_of(const Args& ... values) {
    std::vector<T> v;
    impl::vec_of_impl(v, values...);
    return v;
}

template<class T, class Eq, class Hash, class SetHash>
void random_adds_removes(
        random_container<T, Hash, Eq>& container,
        std::unordered_set<T, SetHash>& set,
        std::mt19937& rnd) {
    for (int i = 0; i < 500; i++) {
        int val = static_cast<int>(rnd() % 500);
        if (rnd() % 2) {
            container.add({val});
            set.insert({val});
        } else {
            container.erase({val});
            set.erase({val});
        }
    }
}

} // namespace correctness

namespace std {

template<>
struct hash<correctness::std_hashable> {
    std::size_t operator()(const correctness::std_hashable& h) const noexcept {
        return std::hash<int>()(static_cast<int>(h));
    }
};

}

namespace compilation {

struct not_default_constructible {
    not_default_constructible(std::string s)
            : s(std::move(s)) {}

    std::string s;

    bool operator==(const not_default_constructible& other) const noexcept {
        return this->s == other.s;
    }

    operator std::string() const {
        return s;
    }
};

template<class RC>
void get_random_element_as_const(const RC& container, int expected) {
    CHECK(expected == container.get_random_element());
}

} // namespace compilation
