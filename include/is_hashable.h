#pragma once

#include <type_traits>

namespace rnd_cnt::impl::is_hashable_impl {

template<class, class T, class Hash>
struct is_hashable_impl : std::false_type {};

template<class T, class Hash>
struct is_hashable_impl<std::void_t<decltype(Hash()(std::declval<T>()))>, T, Hash> : std::true_type {};

} // namespace rnd_cnt::impl::is_hashable_impl

namespace rnd_cnt::impl {

template<class T, class Hash>
using is_hashable = is_hashable_impl::is_hashable_impl<std::void_t<>, T, Hash>;

} // namespace rnd_cnt::impl