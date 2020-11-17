#pragma once

#include <mips/mips.hpp>
#include <mips/register_file.hpp>

namespace mips::testing
{
    class Initializer
    {
    public:
        Initializer(Register left, uint32_t right);

        void invoke(RegisterFile& regs) const;
        friend std::ostream& operator<<(std::ostream& os, const Initializer& initializer);

    private:
        Register _left;
        uint32_t _right;
    };
}
