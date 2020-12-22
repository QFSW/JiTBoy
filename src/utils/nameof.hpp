#pragma once

namespace utils
{
    template <typename T>
    constexpr const char* nameof()
    {
        return typeid(T).name();
    }
}