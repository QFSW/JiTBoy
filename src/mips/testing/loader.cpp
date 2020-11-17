#include "loader.hpp"

#include <iostream>
#include <sstream>
#include <utils/io.hpp>
#include <utils/strtools.hpp>

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
        namespace fs = std::filesystem;
        std::cout << "Loading tests from " << dir_path << "\n";

        std::vector<Test> tests;
        std::vector<fs::path> files = io::get_files_recursive(dir_path);
        std::vector<std::tuple<fs::path, std::string>> failures;

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

                std::stringstream ss;
                ss << e.what() << "\n";
                failures.push_back(std::tuple(file, ss.str()));
            }
        }

        for (const auto& [file, err] : failures)
        {
            std::cout << "\n";
            std::cout << colorize(file.generic_string(), strtools::AnsiColor::Red) << "\n";
            std::cout << err;
        }

        return tests;
    }


}
