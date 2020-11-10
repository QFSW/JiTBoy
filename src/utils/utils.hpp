#pragma once

#include <utility>
#include <utils/detail/final_act.hpp>

namespace utils
{
    template <typename T>
    T copy(const T& t)
    {
        return T(t);
    }

    template <class F>
    [[nodiscard]] detail::final_act<F> finally(const F& f) noexcept
    {
        return detail::final_act(f);
    }

    template <class F>
    [[nodiscard]] detail::final_act<F> finally(F&& f) noexcept
    {
        return detail::final_act(std::move(f));
    }
}