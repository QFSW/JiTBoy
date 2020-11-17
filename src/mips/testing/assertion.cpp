#include "assertion.hpp"

namespace mips::testing
{
    Assertion::Assertion(const Register left, const uint32_t right)
        : _left(left)
        , _right(right)
    { }

    bool Assertion::evaluate(const RegisterFile& regs) const
    {
        return regs[_left] == _right;
    }
}