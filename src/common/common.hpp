#pragma once

#include <libs/tsl/robin_map.h>
#include <libs/tsl/robin_set.h>

namespace common
{
    template <typename TKey, typename TVal>
    using unordered_map = tsl::robin_map<TKey, TVal>;

    template <typename T>
    using unordered_set = tsl::robin_set<T>;
}