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
    code.mov_ir_32(EAX, 4);
    code.mov_ir_32(EBX, 9);
    code.instr<MOV>(Register::EAX, Register::EBX);
    code.instr<ADD>(Register::EAX, Register::EBX);
    code.instr<ADD, InstrMode::RM>(Register::ECX, Register::EAX);
    code.instr<INC>(Register::EAX, UnaryOpcodeExt::INC);
    code.instr<NEG>(Register::EAX, UnaryOpcodeExt::NEG);
    code.instr<RET>();

    auto const buffer = alloc_exe(code.size());
    code.copy(buffer);
    commit_exe(buffer, code.size());

    volatile int ptr[1] = { 5 };
    auto const function_ptr = reinterpret_cast<std::int32_t(*)(void*)>(buffer);
    auto const result = function_ptr(const_cast<int*>(ptr));

    free_exe(buffer);
    std::cout << result << "\n";
    std::cout << *ptr << "\n";

    return 0;
}
