#include "initializer.hpp"

#include <ostream>

namespace mips::testing
{
    Initializer::Initializer(const Register left, const uint32_t right)
        : _left(left)
        , _right(right)
    { }

    void Initializer::invoke(RegisterFile& regs) const
    {
        regs[_left] = _right;
    }

    std::ostream& operator<<(std::ostream& os, const Initializer& initializer)
    {
        os << reg_to_string(initializer._left) << " = " << initializer._right;
        return os;
    }
}