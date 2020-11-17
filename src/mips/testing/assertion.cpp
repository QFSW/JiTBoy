#include "assertion.hpp"

#include <ostream>

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

    std::ostream& operator<<(std::ostream& os, const Assertion& assertion)
    {
        os << reg_to_string(assertion._left) << " == " << assertion._right;
        return os;
    }
}