#include "utils.hpp"

#include <string>

namespace utils
{
    template<> constexpr const char* nameof<uint8_t>() { return "uint8_t"; }
    template<> constexpr const char* nameof<uint16_t>() { return "uint16_t"; }
    template<> constexpr const char* nameof<uint32_t>() { return "uint32_t"; }
    template<> constexpr const char* nameof<uint64_t>() { return "uint64_t"; }

    template<> constexpr const char* nameof<int8_t>() { return "int8_t"; }
    template<> constexpr const char* nameof<int16_t>() { return "int16_t"; }
    template<> constexpr const char* nameof<int32_t>() { return "int32_t"; }
    template<> constexpr const char* nameof<int64_t>() { return "int64_t"; }

    template<> constexpr const char* nameof<std::string>() { return "std::string"; }
}
