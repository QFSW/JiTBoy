#pragma once

#include <mips/register_file.hpp>
#include <mips/memory_map.hpp>
#include <mips/program.hpp>

namespace emulation
{
    struct EmulatorState
    {
        mips::RegisterFile regs;
        mips::MemoryMap mem;
        mips::Program program;
        uint32_t pc = 0;

        [[nodiscard]] bool valid_pc() const noexcept;
    };
}