#include "loader.hpp"

#include <utils/io.hpp>
#include <iostream>

namespace mips::testing
{
    Test Loader::load_test(const std::filesystem::path& filepath)
    {
        const std::string raw = io::read_text_file(filepath.generic_string());
        Test test = _parser.parse(raw);
        test.src = filepath.generic_string();

        if (test.name.empty())
            test.name = filepath.filename().replace_extension("").generic_string();

        return test;
    }

    std::vector<Test> Loader::load_tests(const std::string& dir_path)
    {
        std::cout << "Loading tests from " << dir_path << "\n";

        std::vector<Test> tests;
        std::vector<std::filesystem::path> files = io::get_files_recursive(dir_path);

        tests.reserve(files.size());
        for (const auto& file : files)
        {
            std::cout << "   - " << file.generic_string() << "\n";
            tests.push_back(load_test(file));
        }

        return tests;
    }


}
