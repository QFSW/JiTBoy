#pragma once

#include <mips/instruction.hpp>
#include <emulation/emulator_state.hpp>

namespace emulation
{
    class InterpreterCore
    {
    public:
        explicit InterpreterCore(EmulatorState& state);

        void execute_current();

    private:
        EmulatorState& _state;

        void execute(uint32_t addr);
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
        void execute_mult(mips::InstructionR instr);
        void execute_multu(mips::InstructionR instr);
        void execute_div(mips::InstructionR instr);
        void execute_divu(mips::InstructionR instr);
        void execute_mfhi(mips::InstructionR instr);
        void execute_mflo(mips::InstructionR instr);
        void execute_mthi(mips::InstructionR instr);
        void execute_mtlo(mips::InstructionR instr);
        void execute_slt(mips::InstructionR instr);
        void execute_sltu(mips::InstructionR instr);
        void execute_sll(mips::InstructionR instr);
        void execute_sra(mips::InstructionR instr);
        void execute_srl(mips::InstructionR instr);
        void execute_sllv(mips::InstructionR instr);
        void execute_srav(mips::InstructionR instr);
        void execute_srlv(mips::InstructionR instr);

        void execute_addi(mips::InstructionI instr);
        void execute_addiu(mips::InstructionI instr);
        void execute_andi(mips::InstructionI instr);
        void execute_ori(mips::InstructionI instr);
        void execute_xori(mips::InstructionI instr);
        void execute_slti(mips::InstructionI instr);
        void execute_sltiu(mips::InstructionI instr);
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