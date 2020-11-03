#pragma once

#include <string>
#include <mips/mips.hpp>

namespace mips
{
    Register parse_register(const std::string& reg);
}