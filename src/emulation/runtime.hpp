#pragma once

#include <unordered_map>

#include <config.hpp>
#include <compiler.hpp>
#include <emulation/emulator.hpp>

namespace emulation
{
    class Runtime : public Emulator
    {
    public:
        Runtime();

        void load_source(std::vector<mips::Instruction>&& code, uint32_t addr = instruction_mem_addr);
        void execute(std::vector<mips::Instruction>&& code);
        void execute(uint32_t addr);

        [[nodiscard]] mips::RegisterFile& get_regs() noexcept { return _regs; }
        [[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _regs; }
        [[nodiscard]] const common::unordered_map<uint32_t, CompiledBlock>& get_blocks() const noexcept { return _blocks; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;

    private:
        mips::RegisterFile _regs;
        mips::MemoryMap _mem;
        Compiler _compiler;
        std::vector<mips::Instruction> _source;
        common::unordered_map<uint32_t, CompiledBlock> _blocks;
        uint32_t _pc;

        static constexpr uint32_t instruction_mem_addr = 0x0;
        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] bool valid_pc(uint32_t addr) const noexcept;
        [[nodiscard]] SourceBlock partition_block(uint32_t addr) const;
        [[nodiscard]] const CompiledBlock& get_or_compile_block(uint32_t addr);
    };
}
