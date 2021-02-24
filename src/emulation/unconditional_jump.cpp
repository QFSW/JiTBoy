#include "unconditional_jump.hpp"

namespace emulation
{
    UnconditionalJump::UnconditionalJump(
        uint8_t* src_x86,
        uint32_t dst_mips
    )   : src_x86(src_x86)
        , dst_mips(dst_mips)
        , dst_x86(std::nullopt)
    { }
}
