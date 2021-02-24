#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

#include <config.hpp>
#include <emulation/unconditional_jump.hpp>
#include <emulation/conditional_jump.hpp>
#include <emulation/compiler_config.hpp>

namespace emulation
{
    struct CompiledBlock
    {
        typedef uint32_t(CALLING_CONV* func)();
        CompiledBlock();

        func code;
        size_t size;
        size_t host_instr_count;
        size_t source_instr_count;
        CompilerConfig config;
        std::vector<UnconditionalJump> unresolved_jumps;
        std::vector<ConditionalJump> unresolved_cond_jumps;

        mutable size_t execution_count;
        mutable std::mutex* exec_mutex;

        uint32_t operator()(bool locking = false) const;
    };
}
