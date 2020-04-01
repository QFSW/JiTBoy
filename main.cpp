#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "x86_64.h"
#include "InstructionBuffer.h"

void* alloc_exe(const size_t size)
{
    auto const buffer = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    if (!buffer)
    {
        std::cerr << "exe memory allocation failed" << std::endl;
        std::exit(GetLastError());
    }

    return buffer;
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
    InstructionBuffer code;
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EAX, 0); // EAX = 0
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EDX, 11); // EDX = 11
    code.instr_imm<MOV_I, OpcodeExt::MOV_I>(Register::EBX, 6); // EBX = 6

    const uint32_t size = code.size();
    code.instr<ADD>(Register::EAX, Register::EDX); // EAX += EDX
    code.instr<DEC, OpcodeExt::DEC>(Register::EBX); // EBX--
	
    code.instr_imm<CMP_I, OpcodeExt::CMP_I>(Register::EBX, 0); // Jump back to routine if EBX == 0
    code.jump_cond<CondCode::A>(size - code.size());
	
    code.instr<RET>();

    std::cout << "Generated instructions of size " << code.size() << std::endl;

    auto const buffer = alloc_exe(code.size());
    code.copy(buffer);
    commit_exe(buffer, code.size());

    printf("\n");
	for (size_t i = 0; i < code.size(); ++i)
	{
        printf("%02x ", reinterpret_cast<uint8_t*>(buffer)[i]);
	}
    printf("\n\n");

    std::cout << "Committed instructions to executable memory" << std::endl;
	
    volatile int ptr[2] = { 5 };
    auto const function_ptr = reinterpret_cast<std::int32_t(*)(void*)>(buffer);
    auto const result = function_ptr(const_cast<int*>(ptr));

    std::cout << "Executed instructions" << std::endl;

    free_exe(buffer);
    std::cout << "\nEAX = " << result << "\nptr[0] = " << ptr[0] << "\nptr[1] = " << ptr[1] << std::endl;

    return 0;
}
