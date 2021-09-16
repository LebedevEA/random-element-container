#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <unordered_set>
#include "utils.h"

namespace correctness {

TEST_CASE("simple w/ int") {
    random_container<int> container;
    container.add(1);
    container.add(2);
    container.add(3);
    test_gives_only(container, vec_of<int>(1, 2, 3));
}

TEST_CASE("simple w/ hashable") {
    random_container<std_hashable> container;
    container.add({1});
    container.add({2});
    container.add({3});
    test_gives_only(container, vec_of<std_hashable>(1, 2, 3));
}

TEST_CASE("simple w/ not hashable") {
    random_container<not_hashable> container;
    container.add({1 });
    container.add({2 });
    container.add({3 });
    test_gives_only(container, vec_of<not_hashable>(1, 2, 3));
}

template<class T, class SetHash = std::hash<T>>
void test_case_many_add_remove() {
    std::unordered_set<T, SetHash> included;
    random_container<T> container;
    std::mt19937 rnd(123);

    for (int i = 0; i < 1000; i++) {
        random_adds_removes(container, included, rnd);
        container.add({1});
        included.insert({1});
        test_gives_only(container, included, 250);
    }
}

TEST_CASE("many add/remove w/ int") {
    test_case_many_add_remove<int>();
}

TEST_CASE("many add/remove w/ int") {
    test_case_many_add_remove<std_hashable>();
}

TEST_CASE("many add/remove w/ int") {
    test_case_many_add_remove<not_hashable, not_hashable_hash>();
}

} // namespace correctness

namespace compilation {

TEST_CASE("basic compile") {
    random_container<int> container_hashable;
    container_hashable.add(1);
    auto nodiscard_value = container_hashable.get_random_element();
    container_hashable.erase(1);

    random_container<int, void> container_not_hashable;
    container_not_hashable.add(1);
    nodiscard_value = container_not_hashable.get_random_element();
    container_not_hashable.erase(1);
}

TEST_CASE("not default constructible compile w/o hash") {
    REQUIRE(!std::is_default_constructible_v<not_default_constructible>);
    random_container<not_default_constructible> container;
    container.add({ "str" });
    auto nodiscard_value = container.get_random_element();
    container.erase({ "str" });
}

TEST_CASE("not default constructible compile w/ hash") {
    REQUIRE(!std::is_default_constructible_v<not_default_constructible>);
    random_container<not_default_constructible, std::hash<std::string>> container;
    container.add({ "str" });
    auto nodiscard_value = container.get_random_element();
    container.erase({ "str" });
}

TEST_CASE("works in const") {
    random_container<int> hashable;
    hashable.add(1);
    get_random_element_as_const(hashable, 1);

    random_container<int, void> not_hashable;
    not_hashable.add(2);
    get_random_element_as_const(not_hashable, 2);
}

} // namespace compilation
