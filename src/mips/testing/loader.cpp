#include "loader.hpp"

#include <utils/io.hpp>
#include <utils/strtools.hpp>
#include <iostream>

namespace mips::testing
{
    Test Loader::load_test(const std::filesystem::path& filepath)
    {
        const std::string raw = io::read_text_file(filepath.generic_string());
        Test test = _parser.parse_test(raw);
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
            std::cout << "   - " << file.generic_string();
            try
            {
                tests.push_back(load_test(file));
                std::cout << colorize(" success\n", strtools::AnsiColor::Green);
            }
            catch (const std::exception& e)
            {
                std::cout << colorize(" error\n", strtools::AnsiColor::Red);
                std::cout << e.what() << "\n";
            }
        }

        return tests;
    }


}
