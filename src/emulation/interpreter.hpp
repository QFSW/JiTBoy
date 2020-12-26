#pragma once

#include <sstream>

#include <config.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>
#include <mips/memory_map.hpp>
#include <emulation/emulator.hpp>

namespace emulation
{
    class Interpreter : public Emulator
    {
    public:
        Interpreter();

        void load_source(std::vector<mips::Instruction>&& code, uint32_t addr = instruction_mem_addr);
        void execute(std::vector<mips::Instruction>&& code);
        void execute(uint32_t addr);

        [[nodiscard]] mips::RegisterFile& get_regs() noexcept { return _regs; }
        [[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _regs; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;
        [[nodiscard]] size_t get_instruction_count() const noexcept { return _executed_instructions; }

    private:
        mips::RegisterFile _regs;
        mips::MemoryMap _mem;
        std::vector<mips::Instruction> _source;
        uint32_t _pc;
        size_t _executed_instructions;

        static constexpr uint32_t instruction_mem_addr = 0x0;
        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] bool valid_pc(uint32_t addr) const noexcept;

        void execute(mips::Instruction instr);
        void execute(mips::InstructionR instr);
        void execute(mips::InstructionI instr);
        void execute(mips::InstructionJ instr);

        void throw_invalid_instr(mips::Instruction instr);

        void execute_add(mips::InstructionR instr);
    };
}