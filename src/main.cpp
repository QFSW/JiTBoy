#include <iostream>
#include <fstream>

#include <mips/parsing.hpp>
#include <linker.hpp>
#include <executable_allocator.hpp>
#include <label_generator.hpp>
#include <runtime.hpp>
#include <benchmark.hpp>
#include <utils/utils.hpp>

ExecutableAllocator<4096> allocator;
LabelGenerator label_gen;
Linker linker;

uint32_t return_8()
{
    return 8;
}

int main_test()
{
    const auto assembly =
        "addi $4 $4 55\n"
        "addi $5 $5 5\n"
        "add $3 $4 $5\n"
        "add $0 $4 $5\n"
        "jal 280\n"

        "addi $3 $3 0\n"
        "addi $1 $1 1\n";

    auto code = mips::Parser::parse_instructions(assembly);

    std::cout << "Loaded assembly\n";
    for (const auto& instr : code)
    {
        std::cout << instr << "\n";
    }
    std::cout << "\n";

    auto time = benchmark::measure([&]
    {
        Runtime runtime;
        runtime.execute(utils::copy(code));
        std::cout << runtime.get_debug();
    });

    std::cout << "\nComplete in " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "us" << std::endl;

    return 0;
}

int main()
{
    try
    {
        return main_test();
    }
    catch (std::exception &e)
    {
        std::cerr << "JiTBoy failed!\n" << e.what();
        std::exit(-1);
    }
}