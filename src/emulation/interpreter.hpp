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

        void link(mips::Register reg = mips::Register::$ra);
        void jump(uint32_t target);
        void branch(mips::InstructionI instr);
        uint32_t calc_mem_target(mips::InstructionI instr);

        void execute_add(mips::InstructionR instr);
        void execute_addu(mips::InstructionR instr);
        void execute_sub(mips::InstructionR instr);
        void execute_subu(mips::InstructionR instr);
        void execute_and(mips::InstructionR instr);
        void execute_or(mips::InstructionR instr);
        void execute_nor(mips::InstructionR instr);
        void execute_xor(mips::InstructionR instr);
        void execute_jr(mips::InstructionR instr);
        void execute_jalr(mips::InstructionR instr);
        void execute_mfhi(mips::InstructionR instr);
        void execute_mflo(mips::InstructionR instr);
        void execute_mthi(mips::InstructionR instr);
        void execute_mtlo(mips::InstructionR instr);
        void execute_slt(mips::InstructionR instr);
        void execute_sltu(mips::InstructionR instr);
        void execute_sll(mips::InstructionR instr);
        void execute_sra(mips::InstructionR instr);
        void execute_srl(mips::InstructionR instr);

        void execute_addi(mips::InstructionI instr);
        void execute_addiu(mips::InstructionI instr);
        void execute_andi(mips::InstructionI instr);
        void execute_ori(mips::InstructionI instr);
        void execute_xori(mips::InstructionI instr);
        void execute_lui(mips::InstructionI instr);
        void execute_lw(mips::InstructionI instr);
        void execute_lb(mips::InstructionI instr);
        void execute_lbu(mips::InstructionI instr);
        void execute_lh(mips::InstructionI instr);
        void execute_lhu(mips::InstructionI instr);
        void execute_sw(mips::InstructionI instr);
        void execute_sb(mips::InstructionI instr);
        void execute_sh(mips::InstructionI instr);
        void execute_beq(mips::InstructionI instr);
        void execute_bgtz(mips::InstructionI instr);
        void execute_blez(mips::InstructionI instr);
        void execute_bne(mips::InstructionI instr);
        void execute_bgez(mips::InstructionI instr);
        void execute_bgezal(mips::InstructionI instr);
        void execute_bltz(mips::InstructionI instr);
        void execute_bltzal(mips::InstructionI instr);

        void execute_j(mips::InstructionJ instr);
        void execute_jal(mips::InstructionJ instr);
    };
}