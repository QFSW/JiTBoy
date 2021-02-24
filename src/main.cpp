#include <iostream>

#include <emulation/runtime.hpp>
#include <emulation/interpreter.hpp>
#include <emulation/hybrid_runtime.hpp>
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

mips::testing::Runner::Config parse_config(const int argc, char** argv)
{
    using namespace mips::testing;
    Runner::Config::Timing timing = Runner::Config::Timing::none();

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--timing=none")     timing = Runner::Config::Timing::none();
        if (arg == "--timing=fast")     timing = Runner::Config::Timing::fast();
        if (arg == "--timing=accurate") timing = Runner::Config::Timing::accurate();
        if (arg == "--timing=final")    timing = Runner::Config::Timing::final();
    }

    return Runner::Config{
        .timing = timing
    };
}

void test_bench(const int argc, char** argv)
{
    using namespace mips::testing;
    const std::string tests_csv_path = "output/tests.csv";
    const std::string results_csv_path = "output/results_%s.csv";

    Loader loader;
    Runner runner(parse_config(argc, argv));

    const auto tests = loader.load_tests("tests/mips");
    std::cout << "\n";

    const auto results_jit = runner.run<emulation::Runtime>(tests);
    const auto results_interpreter = runner.run<emulation::Interpreter>(tests);
    const auto results_hybrid = runner.run<emulation::HybridRuntime>(tests);

    std::cout << "\nWriting tests to " + tests_csv_path + "\n";
    csv::write_file(tests_csv_path, tests);

    write_test_results(results_jit, "jit");
    write_test_results(results_interpreter, "interpreter");
    write_test_results(results_hybrid, "hybrid");

    std::cout << "\n";
}

int main(const int argc, char** argv)
{
    try
    {
        if (argc == 3 && std::string(argv[1]) == "--single")
            execute_single(argv[2]);
        else
            test_bench(argc, argv);

        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << "JiTBoy failed!\n" << e.what();
        std::exit(-1);
    }
}
