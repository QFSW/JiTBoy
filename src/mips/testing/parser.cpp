#include "parser.hpp"

#include <stdexcept>
#include <mips/parser.hpp>
#include <utils/strtools.hpp>

namespace mips::testing
{
    std::regex Parser::gen_regex(const std::string& id)
    {
        return std::regex(R"(\s*;\s*)" + id + ":(.+)");
    }

    const std::regex Parser::assert_regex = gen_regex("assert");
    const std::regex Parser::init_regex = gen_regex("init");
    const std::regex Parser::name_regex = gen_regex("name");

    Test Parser::parse(const std::string& raw)
    {
        Test test;
        test.code = mips::Parser::parse_instructions(raw);

        for (const auto& line : strtools::split(raw, '\n'))
        {
            std::smatch matches;
            if (std::regex_search(line, matches, name_regex))
            {
                if (!test.name.empty())
                    throw std::runtime_error("Test cannot contain more than 1 name directive");

                test.name = matches[1];
            }
        }

        return test;
    }

}
