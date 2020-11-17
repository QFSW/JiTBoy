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

int main_test()
{
    constexpr auto asm_path = "resources/test_main.s";
    auto code = mips::Loader::load_assembly(asm_path);

    std::cout << "Loaded assembly\n";
    for (const auto& instr : code)
    {
        std::cout << instr << "\n";
    }
    std::cout << "\n";

    auto time = benchmark::measure([&]
    {
        Runtime runtime;

        auto _ = utils::finally([&] { std::cout << runtime.get_debug(); });
        runtime.execute(utils::copy(code));
    });

    std::cout << "\nComplete in " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "us" << std::endl;

    return 0;
}

int test_bench()
{
    using namespace mips::testing;
    Loader loader;
    Runner runner;

    const auto tests = loader.load_tests("tests/mips");
    std::cout << "\n";
    runner.run(tests);

    std::cout << "\n";
    return 0;
}

int main()
{
    try
    {
        return test_bench();
    }
    catch (std::exception &e)
    {
        std::cerr << "JiTBoy failed!\n" << e.what();
        std::exit(-1);
    }
}