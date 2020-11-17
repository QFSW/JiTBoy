#include "parser.hpp"

#include <stdexcept>
#include <mips/parser.hpp>
#include <utils/strtools.hpp>

namespace mips::testing
{
    std::regex Parser::gen_regex(const std::string& id)
    {
        return std::regex(R"(\s*;\s*)" + id + R"(:\s*(.+))");
    }

    const std::regex Parser::assert_regex = gen_regex("assert");
    const std::regex Parser::init_regex = gen_regex("init");
    const std::regex Parser::name_regex = gen_regex("name");

    Test Parser::parse_test(const std::string& raw)
    {
        Test test;
        for (const auto& line : strtools::split(raw, '\n'))
        {
            std::smatch matches;
            if (std::regex_search(line, matches, name_regex))
            {
                if (!test.name.empty())
                    throw std::runtime_error("Test cannot contain more than 1 name directive");

                test.name = matches[1];
            }

            if (std::regex_search(line, matches, assert_regex))
                test.assertions.push_back(parse_assertion(matches[1]));

            if (std::regex_search(line, matches, init_regex))
                test.initializers.push_back(parse_initializer(matches[1]));
        }

        test.code = mips::Parser::parse_instructions(raw);
        return test;
    }

    Assertion Parser::parse_assertion(const std::string& raw)
    {
        static const std::regex reg(R"(\s*(\S+)\s*==\s*(\S+)\s*)");

        std::smatch matches;
        if (std::regex_search(raw, matches, reg) && matches.size() == 3)
        {
            const Register left = mips::Parser::parse_register(matches[1]);
            const uint32_t right = mips::Parser::parse_constant(matches[2]);
            return Assertion(left, right);
        }

        throw std::runtime_error(raw + " is not a valid assertion");
    }

    Initializer Parser::parse_initializer(const std::string& raw)
    {
        static const std::regex reg(R"(\s*(\S+)\s*=\s*(\S+)\s*)");

        std::smatch matches;
        if (std::regex_search(raw, matches, reg) && matches.size() == 3)
        {
            const Register left = mips::Parser::parse_register(matches[1]);
            const uint32_t right = mips::Parser::parse_constant(matches[2]);
            return Initializer(left, right);
        }

        throw std::runtime_error(raw + " is not a valid initializer");
    }
}
