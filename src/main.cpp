#include <iostream>

#include <runtime.hpp>
#include <benchmark.hpp>
#include <utils/utils.hpp>
#include <mips/loader.hpp>
#include <mips/testing/loader.hpp>
#include <mips/testing/runner.hpp>

uint32_t return_8()
{
    return 8;
}

void execute_single(const std::string& path)
{
    std::cout << "Loading " << path << "\n";

    mips::Loader loader;
    auto code = loader.load_auto(path);

    std::cout << "Loaded assembly\n";
    for (const auto& instr : code)
    {
        std::cout << instr << "\n";
    }
    std::cout << "\n";

    auto time = benchmark::measure([&]
    {
        Runtime runtime;

        auto _ = utils::finally([&] { std::cout << runtime.get_debug_with_dumps(); });
        runtime.execute(utils::copy(code));
    });

    std::cout << "\nComplete in " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "us" << std::endl;
}

void test_bench()
{
    using namespace mips::testing;
    Loader loader;
    Runner runner;

    const auto tests = loader.load_tests("tests/mips");
    std::cout << "\n";
    runner.run(tests);

    std::cout << "\n";
}

int main(const int argc, char** argv)
{
    try
    {
        if (argc > 1)
            execute_single(argv[1]);
        else
            test_bench();

        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << "JiTBoy failed!\n" << e.what();
        std::exit(-1);
    }
}