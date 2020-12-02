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

    [[nodiscard]] mips::RegisterFile& get_regs() noexcept { return _regs; }
    [[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _regs; }
    [[nodiscard]] std::string get_debug() const;
    [[nodiscard]] std::string get_debug_with_dumps() const;
    
private:
    [[nodiscard]] bool valid_pc(uint32_t addr) const noexcept;

    [[nodiscard]] SourceBlock partition_block(uint32_t addr) const;
    [[nodiscard]] CompiledBlock get_or_compile_block(uint32_t addr);

    mips::RegisterFile _regs;
    mips::MemoryMap _mem;
    Compiler::Allocator _allocator;
    Compiler _compiler;
    std::vector<mips::Instruction> _source;
    std::map<uint64_t, CompiledBlock> _blocks;

    uint32_t _current_pc;

    static constexpr uint32_t instruction_mem_addr = 0x0;
    static constexpr bool debug = config::debug;
    std::stringstream _debug_stream;
};

