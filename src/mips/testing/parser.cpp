#include "parser.hpp"

#include <mips/parser.hpp>

namespace mips::testing
{
    Test Parser::parse(const std::string& raw)
    {
        Test test;
        test.code = mips::Parser::parse_instructions(raw);

        return test;
    }

}