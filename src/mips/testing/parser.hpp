#pragma once

#include <regex>
#include <mips/testing/test.hpp>

namespace mips::testing
{
    class Parser
    {
    public:
        Test parse_test(const std::string& raw);
        Assertion parse_assertion(const std::string& raw);
        Initializer parse_initializer(const std::string& raw);

    private:
        static std::regex gen_regex(const std::string& id);

        static const std::regex assert_regex;
        static const std::regex init_regex;
        static const std::regex name_regex;
    };
}
