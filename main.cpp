#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "x86_64.h"
#include "InstructionBuffer.h"
#include "Linker.h"

uint8_t* alloc_exe(const size_t size)
{
    auto const buffer = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    if (!buffer)
    {
        std::cerr << "exe memory allocation failed" << std::endl;
        std::exit(GetLastError());
    }

    return reinterpret_cast<uint8_t*>(buffer);
}

void commit_exe(void* buffer, const size_t size)
{
    DWORD dummy;
    VirtualProtect(buffer, size, PAGE_EXECUTE_READ, &dummy);
}

void free_exe(void* buffer)
{
    VirtualFree(buffer, 0, MEM_RELEASE);
}

void load_binary(std::vector<uint8_t>& vec, const std::string& path)
{
    std::ifstream bin_stream;
    bin_stream.open(path, std::ios_base::binary | std::ios_base::in | std::ios_base::ate);
    if (!bin_stream.is_open())
    {
        std::exit(-1);
    }

    const size_t size = bin_stream.tellg();
    bin_stream.seekg(0, std::ios::beg);

    vec.resize(size);
    bin_stream.read(reinterpret_cast<char*>(vec.data()), size);
}

void dump_binary(const std::vector<uint8_t>& vec, const std::string& path)
{
    std::fstream bin_stream;
    bin_stream.open(path, std::fstream::binary | std::fstream::out);

    bin_stream.write(reinterpret_cast<const char*>(vec.data()), vec.size());
    bin_stream.flush();
    bin_stream.close();
}

int main()
{
    Linker linker;
    InstructionBuffer code;
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EAX, 0); // EAX = 0
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EDX, 11); // EDX = 11
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EBX, 6); // EBX = 6

    code.instr_imm<CMP_I, OpcodeExt::CMP_I>(Register::EBX, 0);
    linker.resolve("loop_skp", [&]{ return code.size(); }, [&](const int32_t offset)
    {
		code.jump_cond<CondCode::E>(offset);
    });
	
    linker.label("loop_start", code.size());
    code.instr<ADD>(Register::EAX, Register::EDX); // EAX += EDX
    code.instr<DEC, OpcodeExt::DEC>(Register::EBX); // EBX--
	
    code.instr_imm<CMP_I, OpcodeExt::CMP_I>(Register::EBX, 0); // Jump back to routine if EBX == 0
    linker.resolve("loop_start", [&] { return code.size(); }, [&](const int32_t offset)
    {
        code.jump_cond<CondCode::A>(offset);
    });

    linker.label("loop_skp", code.size());
    code.instr<MOV, InstrMode::MR>(Register::EAX, Register::ECX);
    code.bswap(Register::EAX);
    code.instr<MOV, InstrMode::RM>(Register::ECX, Register::EAX);
    code.instr<RET>();

    std::cout << "Generated instructions of size " << code.size() << std::endl;

    auto const buffer = alloc_exe(code.size());
    code.copy(buffer);
    linker.terminate_local(buffer);
	
    std::cout << "Globally resolved linker symbols" << std::endl;
    for (const auto& kvp : linker.global_map())
    {
        printf("%s: %p\n", kvp.first.c_str(), kvp.second);
    }
	
    commit_exe(buffer, code.size());

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

    free_exe(buffer);
    std::cout << "\nEAX = " << result;
    for (auto i = 0; i < 4; i++)
    {
        std::cout << "\nptr[" << i << "] = " << static_cast<uint32_t>(ptr[i]);
    }

    std::cout << std::endl;

    return 0;
}
