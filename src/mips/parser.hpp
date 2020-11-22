#pragma once

#include <string>
#include <vector>
#include <regex>

#include <mips/instruction.hpp>
#include <utils/strtools.hpp>
#include <utils/regex_parser.hpp>
#include <utils/variadic.hpp>

namespace mips
{
    class Parser
    {
    public:
        [[nodiscard]] Instruction parse_instruction(const std::string& instr) const;
        [[nodiscard]] std::vector<Instruction> parse_instructions(const std::string& assembly) const;

        static [[nodiscard]] Register parse_register(const std::string& reg);
        static [[nodiscard]] uint32_t parse_constant_32(const std::string& value);
        static [[nodiscard]] uint16_t parse_constant_16(const std::string& value);
        static [[nodiscard]] uint8_t parse_constant_8(const std::string& value);

    private:
        [[nodiscard]] InstructionR parse_nop(const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionR parse_instruction_r(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_sa(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionR parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionR parse_instruction_r_1_src(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionI parse_instruction_i(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionI parse_instruction_i_branch(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionI parse_instruction_i_branch_no_dst(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts) const;
        [[nodiscard]] InstructionJ parse_instruction_j(OpcodeJ opcode, const std::string& instr, const std::vector<std::string>& parts) const;

        static const std::regex comment_regex;
        static constexpr const char* reg_pattern = R"(\$[A-Za-z0-9]+)";
        static constexpr const char* literal_pattern = R"([-+]?[A-Za-z0-9]+)";

        struct Inner
        {
            template <typename T>
            T parse(const std::string& raw)
            {
                throw std::logic_error(strtools::catf("Cannot parse type %s", typeid(T).name()));
            }
        };

        template <typename...Ts>
        static [[nodiscard]] RegexParser<Inner, Ts...> generate_parser(const std::string& pattern);
    };

    template <typename...Ts>
    RegexParser<Parser::Inner, Ts...> Parser::generate_parser(const std::string& pattern)
    {
        static const std::vector<std::string> substitutions =
        {
            ",\\s*",
            "\\s+"
        };

        std::string gen = pattern;
        variadic::loop_types<Ts...>::execute([&](const auto& type)
        {
            const char* p;
            if (type == typeid(Register)) p = reg_pattern;
            else if (type == typeid(uint32_t)) p = literal_pattern;
            else if (type == typeid(uint16_t)) p = literal_pattern;
            else if (type == typeid(uint8_t)) p = literal_pattern;
            else throw std::logic_error(strtools::catf("Cannot generate regex pattern for type %s", type.name()));

            strtools::replace_substr(gen, "??", strtools::catf("(%s)", p));
        });

        for (const auto& sub : substitutions)
        {
            std::regex r(sub);
            gen = std::regex_replace(gen, r, sub);
        }

        return RegexParser<Inner, Ts...>(gen);
    }
}
