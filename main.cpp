#include <windows.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <memory>

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

#define DECOMP_32(x) (x) & 0xFF, ((x) >> 8) & 0xFF, ((x) >> 16) & 0xFF, ((x) >> 24) & 0xFF

#define EAX 0b000
#define AH 0b100

#define MOVI8(reg, x) 0xB0 | (reg), x
#define MOVI32(reg, x) 0xB8 | (reg), DECOMP_32(x)

#define RET 0xC3


int main()
{
    std::vector<uint8_t> const exit_code =
    {
        MOVI32(EAX, 0),
        MOVI8(AH, 2),
        RET
    };

    std::vector<uint8_t> code;
    code.insert(std::end(code), std::begin(exit_code), std::end(exit_code));

    auto const buffer = alloc_exe(code.size());
    std::memcpy(buffer, code.data(), code.size());
    commit_exe(buffer, code.size());

    auto const function_ptr = reinterpret_cast<std::int32_t(*)()>(buffer);
    auto const result = function_ptr();

    free_exe(buffer);
    std::cout << result << "\n";

    return 0;
}
