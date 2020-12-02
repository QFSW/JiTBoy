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

        std::string generate_dump(bool omit_zeroes = true) const;

        uint32_t& operator[](size_t index);
        uint32_t& operator[](Register reg);
        const uint32_t& operator[](Register reg) const;
        const uint32_t& operator[](size_t index) const;
        
    private:
        static constexpr size_t reg_count = 32;
        uint32_t _regs[reg_count] = {};
    };
}

