#pragma once

#include <type_traits>

namespace traits
{
    template <template<class> typename Trait>
    constexpr bool for_all()
    {
        return true;
    }

    template <template<class> typename Trait, typename T, typename...Ts>
    constexpr bool for_all()
    {
        return Trait<T>::value && for_all<Trait, Ts...>();
    }

    template <template<class> typename Trait>
    constexpr bool for_any()
    {
        return false;
    }

    template <template<class> typename Trait, typename T, typename...Ts>
    constexpr bool for_any()
    {
        return Trait<T>::value || for_any<Trait, Ts...>();
    }

    template <template<class> typename Trait, typename...Ts>
    constexpr bool for_none()
    {
        return !for_any<Trait, Ts...>();
    }
}
