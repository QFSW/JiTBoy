#pragma once

#include <sstream>

#include <config.hpp>
#include <mips/instruction.hpp>
#include <emulation/interpreter_core.hpp>
#include <emulation/emulator_state.hpp>
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

        [[nodiscard]] mips::RegisterFile& get_regs() noexcept { return _state.regs; }
        [[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _state.regs; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;
        [[nodiscard]] size_t get_instruction_count() const noexcept { return _executed_instructions; }

    private:
        EmulatorState _state;
        InterpreterCore _core;
        size_t _executed_instructions;

        static constexpr uint32_t instruction_mem_addr = 0x0;
        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] bool valid_pc(uint32_t addr) const noexcept;
    };
}