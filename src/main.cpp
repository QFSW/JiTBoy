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

void execute_single(const int argc, const char** argv)
{
    using namespace mips::testing;
    using namespace emulation;

    Loader loader;
    Runner runner;

    const std::string path = argv[0];
    std::cout << "Loading " << path << "\n";

    const auto test = loader.load_test(path);

    std::cout << "Loaded assembly\n";
    std::cout << test.program << "\n";

    auto time = benchmark::measure([&]
    {
        emulation::Runtime runtime;

        auto _ = utils::finally([&] { std::cout << runtime.get_debug_with_dumps(); });
        runner.execute_test(runtime, test);
    });

    std::cout << "\nComplete in " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "us" << std::endl;
}

void write_test_results(const std::vector<mips::testing::TestResult>& results, const std::string name)
{
    const std::string path = strtools::catf("output/results_%s.csv", name.c_str());

    std::cout << strtools::catf( "Writing %s test results to %s\n", name.c_str(), path.c_str());
    csv::write_file(path, results);
}

mips::testing::Runner::Config parse_config(const int argc, const char** argv)
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

void investigate_hybrid(mips::testing::Runner& runner, const std::vector<mips::testing::Test>& tests)
{
    using namespace emulation;

    std::array<size_t, 5> t_vals = { 1, 10, 100, 1000, 10000 };

    for (const auto t : t_vals)
    {
        HybridRuntime::Config config = {
            .compilation_threshold = t,
            .direct_linking = true,
            .speculative_compilation = false,
        };

        const auto results = runner.run<HybridRuntime>(tests, config);
        write_test_results(results, strtools::catf("hybrid(-L-T%d)", t));
    }
}

void test_bench(const int argc, const char** argv)
{
    using namespace mips::testing;
    using namespace emulation;

    const std::string tests_csv_path = "output/tests.csv";
    const std::string results_csv_path = "output/results_%s.csv";

    Loader loader;
    Runner runner(parse_config(argc, argv));

    const auto tests = loader.load_tests("tests/mips");
    std::cout << "\n";

    const auto results_jit = runner.run<Runtime>(tests);
    const auto results_jit_l = runner.run<Runtime>(tests, Runtime::Config{ .direct_linking = true });
    const auto results_interpreter = runner.run<Interpreter>(tests);
    const auto results_hybrid = runner.run<HybridRuntime>(tests);
    const auto results_hybrid_l = runner.run<HybridRuntime>(tests, HybridRuntime::Config{ .direct_linking = true });
    const auto results_hybrid_ls = runner.run<HybridRuntime>(tests, HybridRuntime::Config{ .direct_linking = true, .speculative_compilation = true });

    std::cout << "\nWriting tests to " + tests_csv_path + "\n";
    csv::write_file(tests_csv_path, tests);

    write_test_results(results_jit, "jit");
    write_test_results(results_jit_l, "jit(-L)");
    write_test_results(results_interpreter, "interpreter");
    write_test_results(results_hybrid, "hybrid");
    write_test_results(results_hybrid_l, "hybrid(-L)");
    write_test_results(results_hybrid_ls, "hybrid(-LS)");

    std::cout << "\n";

    investigate_hybrid(runner, tests);
}

int main(const int argc, const char** argv)
{
    try
    {
        if (argc >= 3 && std::string(argv[1]) == "--single")
            execute_single(argc - 2, argv + 2);
        else
            test_bench(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::cerr << "JiTBoy Failed!\n" << e.what() << std::endl;
        std::exit(-1);
    }

    return 0;
}
