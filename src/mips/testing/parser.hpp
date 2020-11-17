#pragma once

#include <mips/testing/test.hpp>

namespace mips::testing
{
    class Parser
    {
    public:
        Test parse(const std::string& raw);
    };
}