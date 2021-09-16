#pragma once

#include "is_hashable.h"
#include "hashing_impl.h"
#include "non_hashing_impl.h"

namespace rnd_cnt {

template<class T, class Hash = std::hash<T>, class Eq = std::equal_to<T>, class Enable = void>
class random_container;

template<class T, class Hash, class Eq>
class random_container<T, Hash, Eq, typename std::enable_if<impl::is_hashable<T, Hash>::value>::type>
        : public impl::hashing_impl<T, Hash, Eq> {};

template<class T, class Hash, class Eq>
class random_container<T, Hash, Eq, typename std::enable_if<!impl::is_hashable<T, Hash>::value>::type>
        : public impl::non_hashing_impl<T, Eq> {};

} // namespace rnd_cnt
