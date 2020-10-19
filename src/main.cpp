#include <iostream>
#include <fstream>

#include <x86/x86.hpp>
#include <x86/assembler.hpp>
#include <linker.hpp>
#include <executable_allocator.hpp>
#include <label_generator.hpp>

ExecutableAllocator<4096> allocator;
LabelGenerator label_gen;
Linker linker;

uint32_t return_8()
{
    return 8;
}

int main_test()
{
    using namespace x86;
	
    Assembler code;
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EAX, 0); // EAX = 0
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EDX, 11); // EDX = 11
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EBX, 6); // EBX = 6

    code.instr<PUSH, OpcodeExt::PUSH>(Register::EBX);
    code.instr<SUB>(Register::EBX, Register::EBX);
    code.instr<POP, OpcodeExt::POP>(Register::EBX);

    const auto label_skp = label_gen.generate("loop_skp");
    code.instr_imm<CMP_I, OpcodeExt::CMP_I>(Register::EBX, 0);
    linker.resolve(label_skp, [&]{ return code.size(); }, [&](const int32_t offset)
    {
		code.jump_cond<CondCode::E>(offset);
    });

    const auto label_start = label_gen.generate("loop_start");
    linker.label(label_start, code.size());
    code.instr<ADD>(Register::EAX, Register::EDX); // EAX += EDX
    code.instr<DEC, OpcodeExt::DEC>(Register::EBX); // EBX--
	
    code.instr_imm<CMP_I, OpcodeExt::CMP_I>(Register::EBX, 0); // Jump back to routine if EBX == 0
    linker.resolve(label_start, [&] { return code.size(); }, [&](const int32_t offset)
    {
        code.jump_cond<CondCode::A>(offset);
    });

    linker.label(label_skp, code.size());
    code.instr<MOV, InstrMode::MR>(Register::EBX, Register::ECX);
    code.bswap(Register::EBX);
    code.instr<MOV, InstrMode::RM>(Register::ECX, Register::EBX);

    code.enter(16);
    linker.label_global("ret8", return_8);
    linker.resolve("ret8", [&] { return code.size(); }, [&](const int32_t offset)
    {
        code.call(offset);
    });
    code.instr<LEAVE>();
	
    code.instr<RET>();

    std::cout << "Generated instructions of size " << code.size() << std::endl;
    std::cout << code.get_debug() << std::endl;

    auto const buffer = allocator.alloc(code.size());

    std::cout << "Allocated executable memory - "
			  << allocator.get_used() << "b, "
			  << allocator.get_used() * 100 / static_cast<float>(allocator.get_total()) << "% used"
			  << std::endl;
	
    code.copy(buffer);
    linker.terminate_local(buffer);
	
    std::cout << "Globally resolved linker symbols" << std::endl;
    for (const auto& [label, ptr] : linker.global_map())
    {
        printf("%s: %p\n", label.c_str(), ptr);
    }
	
    allocator.commit(buffer, code.size());

    printf("\n");
	for (size_t i = 0; i < code.size(); ++i)
	{
        printf("%02x ", buffer[i]);
	}
    printf("\n\n");

    std::cout << "Committed instructions to executable memory" << std::endl;
	
    volatile uint8_t ptr[4] = { 5, 10, 15, 20 };
    auto const function_ptr = reinterpret_cast<std::int32_t(*)(void*)>(buffer);
    auto const result = function_ptr(const_cast<uint8_t*>(ptr));

    std::cout << "Executed instructions" << std::endl;

    std::cout << "\nEAX = " << result;
    for (auto i = 0; i < 4; i++)
    {
        std::cout << "\nptr[" << i << "] = " << static_cast<uint32_t>(ptr[i]);
    }

    std::cout << std::endl;
	
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