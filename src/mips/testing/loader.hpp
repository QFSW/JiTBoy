#pragma once

#include <vector>
#include <filesystem>
#include <mips/testing/parser.hpp>

namespace mips::testing
{
    class Loader
    {
    public:
        // Add constructor

        [[nodiscard]] Test load_test(const std::filesystem::path& filepath) const;
        [[nodiscard]] std::vector<Test> load_tests(const std::string& dir_path) const;

    private:
        Parser _parser;

        // Add debug buffer
    };
}