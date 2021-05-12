#pragma once

#include <mips/program.hpp>

namespace emulation
{
    class SourceBlock
    {
    public:
        SourceBlock(const mips::Program& program, uint32_t start_addr, uint32_t end_addr);

        [[nodiscard]] uint32_t start_addr() const noexcept;
        [[nodiscard]] uint32_t end_addr() const noexcept;
        [[nodiscard]] uint32_t size() const noexcept;
        [[nodiscard]] bool valid_addr(uint32_t addr) const noexcept;

        const mips::Instruction& at(uint32_t addr) const;

        friend std::ostream& operator<<(std::ostream& os, const SourceBlock& b);

    private:
        const mips::Program& _program;
        uint32_t _start_addr;
        uint32_t _end_addr;
    };
}