#pragma once

#include <optional>

namespace emulation
{
    struct UnconditionalJump
    {
        UnconditionalJump(
            uint8_t* src_x86,
            uint32_t dst_mips
        );

        UnconditionalJump(const UnconditionalJump&) = default;
        UnconditionalJump& operator=(const UnconditionalJump&) = default;

        uint8_t*                src_x86;
        uint32_t                dst_mips;
        std::optional<uint8_t*> dst_x86;
    };
}
