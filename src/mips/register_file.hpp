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
        [[nodiscard]] uint32_t& hi() noexcept { return (*this)[hi_reg]; }
        [[nodiscard]] uint32_t& lo() noexcept { return (*this)[lo_reg]; }
        [[nodiscard]] const uint32_t& hi() const noexcept { return (*this)[hi_reg]; }
        [[nodiscard]] const uint32_t& lo() const noexcept { return (*this)[lo_reg]; }

        std::string generate_dump(bool omit_zeroes = true) const;

        uint32_t& operator[](size_t index);
        uint32_t& operator[](Register reg);
        const uint32_t& operator[](Register reg) const;
        const uint32_t& operator[](size_t index) const;
        
    private:
        static constexpr size_t reg_count = 32;
        uint32_t _regs[reg_count + 2] = {};

    public:
        static constexpr Register hi_reg = static_cast<Register>(reg_count + 0);
        static constexpr Register lo_reg = static_cast<Register>(reg_count + 1);
    };
}

