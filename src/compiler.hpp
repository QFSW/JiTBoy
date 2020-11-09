#pragma once

#include <config.hpp>
#include <executable_allocator.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>
#include <linker.hpp>
#include <label_generator.hpp>
#include <compiled_block.hpp>
#include <source_block.hpp>

class Compiler
{    
public:
    using Allocator = ExecutableAllocator<4096>;
    Compiler(mips::RegisterFile& regs, Allocator& allocator);

    CompiledBlock compile(const SourceBlock& block, CompilerConfig config);
    [[nodiscard]] std::string get_debug() const;
    
private:
    LabelGenerator _label_generator;
    Linker _linker;
    x86::Assembler _assembler;
    mips::RegisterFile& _regs;
    Allocator& _allocator;

    static constexpr x86::Register addr_reg = x86::Register::EDX;
    static constexpr x86::Register return_reg = x86::Register::EAX;

    void compile(mips::Instruction instr, uint32_t addr);
    void compile(mips::InstructionR instr, uint32_t addr);
    void compile(mips::InstructionI instr, uint32_t addr);
    void compile(mips::InstructionJ instr, uint32_t addr);

    void throw_invalid_instr(mips::Instruction instr);

    static constexpr uint32_t calc_reg_offset(mips::Register reg);

    template <x86::Opcode Op = x86::Opcode::MOV>
    void compile_reg_load(x86::Register dst, mips::Register src);

    template <x86::Opcode Op = x86::Opcode::MOV>
    void compile_reg_write(mips::Register dst, x86::Register src);

    template <x86::Opcode Op = x86::Opcode::MOV_I, x86::OpcodeExt Ext = x86::OpcodeExt::MOV_I>
    void compile_reg_write(mips::Register dst, uint32_t imm);

    template <x86::Opcode Op>
    void compile(mips::InstructionR instr);

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void compile(mips::InstructionI instr);

    void compile_call(void (*f)());
    void compile_jump(uint32_t target);

    template <typename T, void(T::* F)()>
    void compile_call(T& obj);

    template <typename T, void(T::* F)()>
    static void __fastcall proxy_call(T* obj);

    template <typename T, typename A, void(T::* F)(A)>
    static void __fastcall proxy_call_arg(T* obj, A a);

    static constexpr bool debug = config::debug;
    std::stringstream _debug_stream;
};

template <typename T, void(T::* F)()>
void Compiler::compile_call(T& obj)
{
    using namespace x86;
    const auto addr = reinterpret_cast<uint32_t>(&obj);
    _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, addr);

    const auto label = _label_generator.generate("member_func");
    _linker.label_global(label, &proxy_call<T, F>);
    _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
    {
        _assembler.call(offset);
    });
}

template <typename T, void(T::* F)()>
void __fastcall Compiler::proxy_call(T* obj)
{
    return (obj->*F)();
}

template <typename T, typename A, void(T::* F)(A)>
void __fastcall Compiler::proxy_call_arg(T* obj, A a)
{
    return (obj->*F)(a);
}
