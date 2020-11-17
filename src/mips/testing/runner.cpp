#include "runner.hpp"

#include <map>
#include <iostream>
#include <sstream>
#include <runtime.hpp>
#include <utils/strtools.hpp>

namespace mips::testing
{
    void Runner::run(const std::vector<Test>& tests)
    {
        std::cout << "Running tests\n";

        size_t pass_count = 0;
        std::vector<std::tuple<Test, std::string>> failures;

        for (const auto& test : tests)
        {
            Runtime runtime;
            std::cout << "   - " << test.name;

            try
            {
                for (const auto& initializer : test.initializers)
                    initializer.invoke(runtime.get_regs());

                runtime.execute(utils::copy(test.code));

                bool failed = false;
                std::stringstream ss;
                for (const auto& assertion : test.assertions)
                {
                    if (!assertion.evaluate(runtime.get_regs()))
                    {
                        if (!failed)
                            std::cout << colorize(" failed\n", strtools::AnsiColor::Red);

                        failed = true;
                        ss << "Assertion failed: " << assertion << "\n";
                    }
                }

                if (failed)
                {
                    ss << runtime.get_debug();
                    failures.push_back(std::tuple(test, ss.str()));
                }
                else
                {
                    pass_count++;
                    std::cout << colorize(" pass\n", strtools::AnsiColor::Green);
                }
            }
            catch (const std::exception& e)
            {
                std::cout << colorize(" faulted\n", strtools::AnsiColor::Red);

                std::stringstream ss;
                ss << e.what() << "\n";
                ss << runtime.get_debug();
                failures.push_back(std::tuple(test, ss.str()));
            }
        }

        std::cout << strtools::catf("\n%d/%d tests passed\n", pass_count, tests.size());

        for (const auto& [test, err] : failures)
        {
            std::cout << "\n";
            std::cout << strtools::colorize(test.name, strtools::AnsiColor::Red) << "\n";
            std::cout << test.description << "\n";
            std::cout << err;
        }
    }

}
