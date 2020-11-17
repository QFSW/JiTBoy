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

        Test load_test(const std::filesystem::path& filepath);
        std::vector<Test> load_tests(const std::string& dir_path);

    private:
        Parser _parser;

        // Add debug buffer
    };
}