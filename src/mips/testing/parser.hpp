#pragma once

#include <regex>
#include <mips/testing/test.hpp>
#include <mips/parser.hpp>

namespace mips::testing
{
    class Parser
    {
    public:
        Test parse_test(const std::string& raw) const;
        Assertion parse_assertion(const std::string& raw) const;
        Initializer parse_initializer(const std::string& raw) const;

    private:
        mips::Parser _mips_parser;

        static std::regex gen_regex(const std::string& id);

        static const std::regex assert_regex;
        static const std::regex init_regex;
        static const std::regex name_regex;
        static const std::regex desc_regex;

        static constexpr const char* reg_pattern = R"(\$[A-Za-z0-9]+)";
        static constexpr const char* literal_pattern = R"([-+]?[A-Za-z0-9]+)";
    };
}
