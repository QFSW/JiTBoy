#pragma once

#include <mips/program.hpp>
#include <mips/parser.hpp>

namespace mips
{
    class Loader
    {
    public:
        [[nodiscard]] Program load_assembly(const std::string& filepath);
        [[nodiscard]] Program load_binary(const std::string& filepath);
        [[nodiscard]] Program load_auto(const std::string& filepath);

    private:
        Parser _parser;
    };
}

