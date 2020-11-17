#pragma once

#include <mips/mips.hpp>
#include <mips/register_file.hpp>

namespace mips::testing
{
    class Assertion
    {
    public:
        Assertion(Register left, uint32_t right);

        [[nodiscard]] bool evaluate(const RegisterFile& regs) const;
        friend std::ostream& operator<<(std::ostream& os, const Assertion& assertion);

    private:
        Register _left;
        uint32_t _right;
    };
}
