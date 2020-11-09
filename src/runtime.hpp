#pragma once

#include <map>

#include <config.hpp>
#include <compiler.hpp>

class Runtime
{
public:
    Runtime();

    void load_source(std::vector<mips::Instruction>&& code, uint32_t addr = instruction_mem_addr);
    void execute(std::vector<mips::Instruction>&& code);
    void execute(uint32_t addr);
    
    [[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _regs; }
    [[nodiscard]] std::string get_debug() const;
    
private:
    SourceBlock partition_block(uint32_t addr) const;
    CompiledBlock get_or_compile_block(uint32_t addr);

    mips::RegisterFile _regs;
    Compiler::Allocator _allocator;
    Compiler _compiler;
    std::vector<mips::Instruction> _source;
    std::map<uint64_t, CompiledBlock> _blocks;

    static constexpr uint32_t instruction_mem_addr = 0x100;
    static constexpr bool debug = config::debug;
    std::stringstream _debug_stream;
};

