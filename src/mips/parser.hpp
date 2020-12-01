#pragma once

#include <string>
#include <vector>
#include <map>
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
        [[nodiscard]] std::vector<Instruction> parse_instructions(const std::string& assembly);

        static [[nodiscard]] Register parse_register(const std::string& reg);
        static [[nodiscard]] uint32_t parse_constant_32(const std::string& value);
        static [[nodiscard]] uint16_t parse_constant_16(const std::string& value);
        static [[nodiscard]] uint8_t parse_constant_8(const std::string& value);

    private:
        uint32_t _pc = 0;
        std::map<std::string, uint32_t> _labels;

        void reset();
        void extract_labels(std::string& raw);
        [[nodiscard]] Instruction parse_instruction(const std::string& instr);

        [[nodiscard]] InstructionR parse_nop(const std::string& instr) const;
        [[nodiscard]] InstructionR parse_jalr(const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_sa(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_1_src(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_1_src(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_branch(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_branch_no_dst(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_memory(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionJ parse_instruction_j(OpcodeJ opcode, const std::string& instr) const;

        static const std::regex comment_regex;
        static constexpr const char* reg_pattern = R"(\$[A-Za-z0-9]+)";
        static constexpr const char* literal_pattern = R"([-+]?[A-Za-z0-9]+)";
        static constexpr const char* default_parser_err = "Invalid instruction %s";

        struct Inner
        {
            template <typename T>
            T parse(const std::string& raw)
            {
                throw std::logic_error(strtools::catf("Cannot parse type %s", typeid(T).name()));
            }
        };

        template <typename...Ts>
        static [[nodiscard]] RegexParser<Inner, Ts...> generate_parser(const std::string& pattern, std::string&& err_string = default_parser_err);
    };

    template <typename...Ts>
    RegexParser<Parser::Inner, Ts...> Parser::generate_parser(const std::string& pattern, std::string&& err_string)
    {
        static const std::vector<std::string> substitutions =
        {
            ",\\s*",
            "\\s+"
        };

        std::string gen = pattern;

        variadic::loop_types<Ts...>::execute([&](const auto& type)
        {
            if (!strtools::str_contains(gen, "??"))
                return;

            const char* p = nullptr;
            if (type == typeid(Register)) p = reg_pattern;
            else if (type == typeid(uint32_t)) p = literal_pattern;
            else if (type == typeid(uint16_t)) p = literal_pattern;
            else if (type == typeid(uint8_t)) p = literal_pattern;
            else if (type == typeid(int32_t)) p = literal_pattern;
            else if (type == typeid(int16_t)) p = literal_pattern;
            else if (type == typeid(int8_t)) p = literal_pattern;
            else throw std::logic_error(strtools::catf("Cannot generate regex pattern for type %s", type.name()));

            strtools::replace_substr(gen, "??", strtools::catf("(%s)", p));
        });

        for (const auto& sub : substitutions)
        {
            std::regex r(sub);
            gen = std::regex_replace(gen, r, sub);
        }

        gen = strtools::catf(R"(^\s*%s\s*$)", gen.c_str());
        std::regex reg(gen, std::regex_constants::optimize);
        return RegexParser<Inner, Ts...>(std::move(reg), std::move(err_string));
    }
}
