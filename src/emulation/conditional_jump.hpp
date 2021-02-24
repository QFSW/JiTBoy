#pragma once

#include <optional>
#include <x86/x86.hpp>

namespace emulation
{
    struct ConditionalJump
    {
        ConditionalJump(
            uint8_t* src_x86,
            x86::CondCode cond,
            uint32_t dst_true_mips,
            uint32_t dst_false_mips
        );

        ConditionalJump(const ConditionalJump&) = default;
        ConditionalJump& operator=(const ConditionalJump&) = default;

        uint8_t*                src_x86;
        x86::CondCode           cond;
        uint32_t                dst_true_mips;
        uint32_t                dst_false_mips;
        std::optional<uint8_t*> dst_true_x86;
        std::optional<uint8_t*> dst_false_x86;
    };
}
