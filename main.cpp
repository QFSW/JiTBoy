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

int main()
{
    InstructionBuffer code;
    code.write_mov_ir_32(EAX, 0);
    code.write_mov_ir_8(AL, 9);
    code.write_raw(RET);

    auto const buffer = alloc_exe(code.size());
    code.copy(buffer);
    commit_exe(buffer, code.size());

    auto const function_ptr = reinterpret_cast<std::int32_t(*)()>(buffer);
    auto const result = function_ptr();

    free_exe(buffer);
    std::cout << result << "\n";

    return 0;
}
