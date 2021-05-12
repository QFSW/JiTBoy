#pragma once

#include <common/common.hpp>
#include <mips/instruction.hpp>
#include <map>

namespace mips
{
    class Program
    {
    public:
        Program();
        Program(common::unordered_map<uint32_t, Instruction>&& source, uint32_t entry_point);

        [[nodiscard]] uint32_t entry_point() const noexcept { return _entry_point; }
        [[nodiscard]] bool valid_addr(uint32_t addr) const noexcept;
        [[nodiscard]] const Instruction& at(uint32_t addr) const;

        friend std::ostream& operator<<(std::ostream& os, const Program& p);

    private:
        common::unordered_map<uint32_t, Instruction> _source;
        uint32_t _entry_point;

        mutable std::map<uint32_t, Instruction> _ordered_cache;
    };
}
