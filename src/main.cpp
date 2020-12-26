#include <iostream>

#include <emulation/runtime.hpp>
#include <emulation/interpreter.hpp>
#include <utils/benchmark.hpp>
#include <utils/utils.hpp>
#include <utils/csv.hpp>
#include <utils/strtools.hpp>
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
        emulation::Runtime runtime;

        auto _ = utils::finally([&] { std::cout << runtime.get_debug_with_dumps(); });
        runtime.execute(utils::copy(code));
    });

    std::cout << "\nComplete in " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "us" << std::endl;
}

void write_test_results(const std::vector<mips::testing::TestResult>& results, const std::string name)
{
    const std::string path = strtools::catf("output/results_%s.csv", name.c_str());

    std::cout << strtools::catf( "Writing %s test results to %s\n", name.c_str(), path.c_str());
    csv::write_file(path, results);
}

void test_bench()
{
    using namespace mips::testing;
    const std::string tests_csv_path = "output/tests.csv";
    const std::string results_csv_path = "output/results_%s.csv";

    Loader loader;
    Runner runner;

    const auto tests = loader.load_tests("tests/mips");
    std::cout << "\n";

    const auto results_jit = runner.run<emulation::Runtime>(tests);
    const auto results_interpreter = runner.run<emulation::Interpreter>(tests);

    std::cout << "\nWriting tests to " + tests_csv_path + "\n";
    csv::write_file(tests_csv_path, tests);

    write_test_results(results_jit, "jit");
    write_test_results(results_interpreter, "interpreter");

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