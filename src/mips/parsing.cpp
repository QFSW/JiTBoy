#include "parsing.hpp"

#include <map>
#include <stdexcept>

namespace mips
{
    Register parse_register(const std::string& reg)
    {
        static const std::map<std::string, Register> reg_mapping =
        {
            { "$0", Register::r0 },
            { "$1", Register::r1 },
            { "$2", Register::r2 },
            { "$3", Register::r3 },
            { "$4", Register::r4 },
            { "$5", Register::r5 },
            { "$6", Register::r6 },
            { "$7", Register::r7 },
            { "$8", Register::r8 },
            { "$9", Register::r9 },
            { "$10", Register::r10 },
            { "$11", Register::r11 },
            { "$12", Register::r12 },
            { "$13", Register::r13 },
            { "$14", Register::r14 },
            { "$15", Register::r15 },
            { "$16", Register::r16 },
            { "$17", Register::r17 },
            { "$18", Register::r18 },
            { "$19", Register::r19 },
            { "$20", Register::r20 },
            { "$21", Register::r21 },
            { "$22", Register::r22 },
            { "$23", Register::r23 },
            { "$24", Register::r24 },
            { "$25", Register::r25 },
            { "$26", Register::r26 },
            { "$27", Register::r27 },
            { "$28", Register::r28 },
            { "$29", Register::r29 },
            { "$30", Register::r30 },
            { "$31", Register::r31 }
        };

        if (reg_mapping.find(reg) == reg_mapping.end())
        {
            throw std::logic_error("Could not parse register " + reg);
        }

        return reg_mapping.at(reg);
    }
}