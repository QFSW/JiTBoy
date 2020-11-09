#include <iostream>
#include <fstream>

#include <x86/x86.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/encoding.hpp>
#include <mips/parsing.hpp>
#include <linker.hpp>
#include <executable_allocator.hpp>
#include <label_generator.hpp>
#include <runtime.hpp>
#include <benchmark.hpp>

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
        "add $0 $4 $5\n";

    const auto code = mips::Parser::parse_instructions(assembly);

    auto time = benchmark::measure([&]
    {
        Runtime runtime;
        runtime.execute(code);
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