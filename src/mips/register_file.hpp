#pragma once
#include <mips/mips.hpp>

namespace mips
{
    class RegisterFile
    {
    public:
        [[nodiscard]] size_t size() const noexcept { return reg_count; }
        [[nodiscard]] uint32_t* data() noexcept { return _regs; }
        [[nodiscard]] const uint32_t* data() const noexcept { return _regs; }

        uint32_t& operator[] (const size_t index) { return _regs[index]; }
        uint32_t& operator[] (Register reg) { return _regs[static_cast<size_t>(reg)]; }
        const uint32_t& operator[] (Register reg) const { return _regs[static_cast<size_t>(reg)]; }
        const uint32_t& operator[] (const size_t index) const { return _regs[index]; }
        
    private:
        static constexpr size_t reg_count = 32;
        uint32_t _regs[reg_count] = {};
    };
}

