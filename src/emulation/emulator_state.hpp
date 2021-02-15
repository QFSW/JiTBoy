#pragma once

#include <mips/register_file.hpp>
#include <mips/memory_map.hpp>
#include <mips/instruction.hpp>

namespace emulation
{
    struct EmulatorState
    {
        mips::RegisterFile regs;
        mips::MemoryMap mem;
        std::vector<mips::Instruction> source;
        uint32_t pc = 0;
    };
}