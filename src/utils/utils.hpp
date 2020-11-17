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

    template <typename...Args>
    struct instance_proxy
    {
        template <typename T, typename R, R(T::* F)(Args...)>
        static R __fastcall call(T* obj, Args...args)
        {
            return (obj->*F)(std::forward(args...));
        }
    };
}