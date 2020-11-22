#pragma once

#include <utility>

namespace variadic
{
    template <typename F, typename T>
    constexpr T fold(F&& f, T t)
    {
        return t;
    }

    template <typename F, typename T1, typename T2, typename...Ts>
    constexpr T1 fold(F&& f, T1 t1, T2 t2, Ts...ts)
    {
        T1 t = f(t1, t2);
        return fold(std::forward(f), t, std::forward(ts...));
    }

    template <typename F, typename T>
    constexpr void loop(F&& f, T t)
    {
        f(t);
    }

    template <typename F, typename T, typename...Ts>
    constexpr void loop(F&& f, T t, Ts...ts)
    {
        f(t);
        loop(std::forward(f), std::forward(ts...));
    }

    template <typename T, typename... Ts>
    struct loop_types
    {
        template <typename F>
        static constexpr void execute(F&& f)
        {
            f(typeid(T));
            loop_types<Ts...>::execute(f);
        }
    };

    template <typename T>
    struct loop_types<T>
    {
        template <typename F>
        static constexpr void execute(F&& f)
        {
            f(typeid(T));
        }
    };
}