#pragma once

namespace utils
{
    template <typename T>
    T copy(const T& t)
    {
        return T(t);
    }
}