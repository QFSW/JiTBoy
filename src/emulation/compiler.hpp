#pragma once

#include <config.hpp>
#include <emulation/source_block.hpp>
#include <emulation/compiled_block.hpp>
#include <emulation/compiler_config.hpp>
#include <x86/assembler.hpp>
#include <x86/linker.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>
#include <mips/memory_map.hpp>
#include <memory/dynamic_executable_allocator.hpp>
#include <label_generator.hpp>

namespace emulation
{
    class Compiler
    {
    public:
        using Config = CompilerConfig;

        Compiler(mips::RegisterFile& regs, mips::MemoryMap& mem);

        CompiledBlock compile(const SourceBlock& block, Config config);
        void resolve_jumps(CompiledBlock& block, const common::unordered_map<uint32_t, CompiledBlock>& blocks);
        [[nodiscard]] std::string get_debug() const;

    private:
        using Allocator = memory::DynamicExecutableAllocator<4096>;

        Allocator _allocator;
        LabelGenerator _label_generator;
        x86::Linker _linker;
        x86::Assembler _assembler;
        mips::RegisterFile& _regs;
        mips::MemoryMap& _mem;
        std::mutex _exec_mem_mutex;

        std::vector<std::tuple<size_t, uint32_t>> _unresolved_jumps;

        static constexpr x86::Register addr_reg = x86::Register::ECX;
        static constexpr x86::Register return_reg = x86::Register::EAX;
        static constexpr x86::Register acc1_reg = x86::Register::EAX;
        static constexpr x86::Register acc2_reg = x86::Register::EDX;

        void reset();

        void compile(mips::Instruction instr, uint32_t addr);
        void compile(mips::InstructionR instr, uint32_t addr);
        void compile(mips::InstructionI instr, uint32_t addr);
        void compile(mips::InstructionJ instr, uint32_t addr);

        void throw_invalid_instr(mips::Instruction instr);

        static constexpr uint32_t calc_reg_offset(mips::Register reg);

        template <x86::Opcode Op = x86::Opcode::MOV>
        void compile_reg_load(x86::Register dst, mips::Register src);

        template <x86::Opcode Op = x86::Opcode::MOV_I, x86::OpcodeExt Ext = x86::OpcodeExt::MOV_I>
        void compile_reg_read(mips::Register src, uint32_t imm);

        template <x86::Opcode Op = x86::Opcode::MOV>
        void compile_reg_write(mips::Register dst, x86::Register src);

        template <x86::Opcode Op = x86::Opcode::MOV_I, x86::OpcodeExt Ext = x86::OpcodeExt::MOV_I>
        void compile_reg_write(mips::Register dst, uint32_t imm);

        template <x86::Opcode Op>
        void compile(mips::InstructionR instr);

        template <x86::Opcode Op, x86::OpcodeExt Ext>
        void compile(mips::InstructionI instr);

        template <x86::Opcode Op, x86::OpcodeExt Ext>
        void compile_imm(mips::Register dst, mips::Register src, uint32_t imm);

        template <x86::CondCode Cond>
        void compile_compare(mips::InstructionR instr);

        template <x86::CondCode Cond>
        void compile_compare(mips::InstructionI instr);

        template <x86::CondCode Cond>
        void compile_branch(mips::InstructionI instr, uint32_t addr);

        template <x86::CondCode Cond>
        void compile_branch_and_link(mips::InstructionI instr, uint32_t addr);

        template <x86::Opcode Op, x86::OpcodeExt Ext>
        void compile_shift_imm(mips::InstructionR instr);

        template <x86::Opcode Op, x86::OpcodeExt Ext>
        void compile_shift(mips::InstructionR instr);

        template <x86::Opcode Op, x86::OpcodeExt Ext>
        void compile_mul(mips::InstructionR instr);

        void compile_div(mips::InstructionR instr);
        void compile_divu(mips::InstructionR instr);
        void compile_hi_lo_writeback();

        void compile_nor(mips::InstructionR instr);
        void compile_mfhi(mips::InstructionR instr);
        void compile_mflo(mips::InstructionR instr);
        void compile_mthi(mips::InstructionR instr);
        void compile_mtlo(mips::InstructionR instr);
        void compile_call(void (*f)());
        void compile_jump(uint32_t target);
        void compile_jump(mips::Register target);
        void compile_jump_and_link(mips::Register target, mips::Register link_reg, uint32_t addr);
        void compile_compute_mem_addr(x86::Register dst, mips::InstructionI instr);

        template <typename T = uint32_t>
        void compile_mem_write(mips::InstructionI instr);

        template <typename T = uint32_t>
        void compile_mem_read(mips::InstructionI instr);

        template <typename T, void(T::* F)()>
        void compile_call(T& obj);

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;
    };
}
