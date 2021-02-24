#include "utils.hpp"

#include <stdexcept>

namespace x86::utils
{
    CondCode negate_cond(const CondCode cond)
    {
        switch (cond)
        {
            case CondCode::O:  return CondCode::NO;
            case CondCode::B:  return CondCode::NB;
            case CondCode::E:  return CondCode::NE;
            case CondCode::A:  return CondCode::NA;
            case CondCode::S:  return CondCode::NS;
            case CondCode::P:  return CondCode::NP;
            case CondCode::L:  return CondCode::NL;
            case CondCode::G:  return CondCode::NG;
            case CondCode::NO: return CondCode::O;
            case CondCode::NB: return CondCode::B;
            case CondCode::NE: return CondCode::E;
            case CondCode::NA: return CondCode::A;
            case CondCode::NS: return CondCode::S;
            case CondCode::NP: return CondCode::P;
            case CondCode::NL: return CondCode::L;
            case CondCode::NG: return CondCode::G;
        }

        throw std::logic_error("Cannot negate condition");
    }
}
