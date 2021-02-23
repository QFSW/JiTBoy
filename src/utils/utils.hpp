#pragma once

#include <cstdint>
#include <mutex>
#include <utility>
#include <utils/detail/final_act.hpp>

#include <config.hpp>

namespace utils
{
    template <typename T>
    [[nodiscard]] T copy(const T& t)
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
        static R CALLING_CONV call(T* obj, Args...args)
        {
            return (obj->*F)(args...);
        }
    };

    template <typename T>
    constexpr const char* nameof()
    {
        return typeid(T).name();
    }

    template<> constexpr const char* nameof<uint8_t>() { return "uint8_t"; }
    template<> constexpr const char* nameof<uint16_t>() { return "uint16_t"; }
    template<> constexpr const char* nameof<uint32_t>() { return "uint32_t"; }
    template<> constexpr const char* nameof<uint64_t>() { return "uint64_t"; }

    template<> constexpr const char* nameof<int8_t>() { return "int8_t"; }
    template<> constexpr const char* nameof<int16_t>() { return "int16_t"; }
    template<> constexpr const char* nameof<int32_t>() { return "int32_t"; }
    template<> constexpr const char* nameof<int64_t>() { return "int64_t"; }

    template<> constexpr const char* nameof<std::string>() { return "std::string"; }

    template <typename T>
    void swap(T& t1, T& t2) noexcept
    {
        T tmp = std::move(t1);
        t1 = std::move(t2);
        t2 = std::move(tmp);
    }

    template <typename T>
    [[nodiscard]] T bswap(T value)
    {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);
        for (size_t i = 0; i < sizeof(T) / 2; ++i)
            swap(bytes[i], bytes[sizeof(T) - 1 - i]);

        return value;
    }

    template<> inline uint8_t  bswap(const uint8_t value) { return value; }
    template<> inline int8_t   bswap(const int8_t value) { return value; }
    template<> inline uint16_t bswap(const uint16_t value) { return _byteswap_ushort(value); }
    template<> inline int16_t  bswap(const int16_t value) { return _byteswap_ushort(value); }
    template<> inline uint32_t bswap(const uint32_t value) { return _byteswap_ulong(value); }
    template<> inline int32_t  bswap(const int32_t value) { return _byteswap_ulong(value); }
    template<> inline uint64_t bswap(const uint64_t value) { return _byteswap_uint64(value); }
    template<> inline int64_t  bswap(const int64_t value) { return _byteswap_uint64(value); }

    template <typename F>
    void potentially_lock(std::mutex& m, const bool should_lock, F&& f)
    {
        if (should_lock)
        {
            std::lock_guard<std::mutex> lock(m);
            f();
        }
        else
        {
            f();
        }
    }

    template <typename T, typename F>
    T potentially_lock_return(std::mutex& m, const bool should_lock, F&& f)
    {
        if (should_lock)
        {
            std::lock_guard<std::mutex> lock(m);
            return f();
        }

        return f();
    }
}