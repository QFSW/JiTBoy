#include "conditional_jump.hpp"

namespace emulation
{
    ConditionalJump::ConditionalJump(
        uint8_t* src_x86,
        x86::CondCode cond,
        uint32_t dst_true_mips,
        uint32_t dst_false_mips
    ) : src_x86(src_x86)
        , cond(cond)
        , dst_true_mips(dst_true_mips)
        , dst_false_mips(dst_false_mips)
        , dst_true_x86(std::nullopt)
        , dst_false_x86(std::nullopt)
    { }
}
