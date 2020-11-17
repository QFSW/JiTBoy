#pragma once

#include <mips/mips.hpp>
#include <mips/register_file.hpp>

namespace mips::testing
{
    class Assertion
    {
    public:
        Assertion(Register left, uint32_t right);

        bool evaluate(const RegisterFile& regs) const;
    private:
        Register _left;
        uint32_t _right;
    };
}
