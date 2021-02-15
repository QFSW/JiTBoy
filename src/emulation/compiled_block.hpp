#pragma once

#include <cstdint>
#include <config.hpp>
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

        mutable size_t execution_count;

        uint32_t operator()() const;
    };
}
