#pragma once

#include <type_traits>

namespace traits
{
    template <template<class> typename Trait, typename...Ts>
    constexpr bool for_all()
    {
        return (Trait<Ts>::value && ...);
    }

    template <template<class> typename Trait, typename...Ts>
    constexpr bool for_any()
    {
        return (Trait<Ts>::value || ...);
    }

    template <template<class> typename Trait, typename...Ts>
    constexpr bool for_none()
    {
        return !for_any<Trait, Ts...>();
    }
}
