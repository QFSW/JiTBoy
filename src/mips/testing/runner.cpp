#include "runner.hpp"

#include <iostream>
#include <runtime.hpp>
#include <utils/strtools.hpp>

namespace mips::testing
{
    void Runner::run(const std::vector<Test>& tests)
    {
        std::cout << "Running tests\n";

        size_t pass_count = 0;
        for (const auto& test : tests)
        {
            Runtime runtime;
            std::cout << "   - " << test.name;

            try
            {
                for (const auto& initializer : test.initializers)
                    initializer.invoke(runtime.get_regs());

                runtime.execute(utils::copy(test.code));
                pass_count++;

                bool failed = false;
                for (const auto& assertion : test.assertions)
                {
                    if (!assertion.evaluate(runtime.get_regs()))
                    {
                        if (!failed)
                            std::cout << colorize(" failed\n", strtools::AnsiColor::Red);

                        failed = true;
                        std::cout << "Assertion failed: " << assertion << "\n";
                    }
                }

                if (!failed)
                    std::cout << colorize(" pass\n", strtools::AnsiColor::Green);
            }
            catch (const std::exception& e)
            {
                std::cout << colorize(" faulted\n", strtools::AnsiColor::Red);
                std::cout << e.what() << "\n";
                std::cout << runtime.get_debug();
            }
        }

        std::cout << strtools::catf("\n%d/%d tests passed", pass_count, tests.size());
    }

}
