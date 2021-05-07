#pragma once

#include <string>
#include <vector>
#include <regex>

#include <mips/program.hpp>
#include <mips/instruction.hpp>
#include <utils/strtools.hpp>
#include <utils/regex_parser.hpp>
#include <utils/variadic.hpp>
#include <common/common.hpp>

namespace mips
{
    class Parser
    {
    public:
        [[nodiscard]] Program parse_program(const std::string& assembly);

        static [[nodiscard]] Register parse_register(const std::string& reg);
        static [[nodiscard]] uint32_t parse_constant_32(const std::string& value);
        static [[nodiscard]] uint16_t parse_constant_16(const std::string& value);
        static [[nodiscard]] uint8_t parse_constant_8(const std::string& value);

    private:
        struct State
        {
            uint32_t start_addr = 0;
            uint32_t pc = 0;

            common::unordered_map<std::string, uint32_t> labels;
            common::unordered_map<uint32_t, std::string> unresolved_locals;
        };

        State _state;

        void reset();
        void extract_labels(std::string& raw);
        bool try_resolve_label(const std::string& label, uint32_t& addr) const;

        [[nodiscard]] uint32_t parse_target_abs(const std::string& raw, uint32_t pc, bool can_defer = true);
        [[nodiscard]] int16_t parse_target_rel(const std::string& raw, uint32_t pc, bool can_defer = true);

        [[nodiscard]] Instruction parse_instruction(const std::string& instr);
        [[nodiscard]] InstructionR parse_nop(const std::string& instr) const;
        [[nodiscard]] InstructionR parse_jalr(const std::string& instr) const;
        [[nodiscard]] InstructionI parse_subi(const std::string& instr) const;
        [[nodiscard]] InstructionI parse_b(const std::string& instr);
        [[nodiscard]] InstructionI parse_bal(const std::string& instr);
        [[nodiscard]] Instruction  parse_move(const std::string& instr) const;
        [[nodiscard]] InstructionR parse_jr(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_hi_lo(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_sxxv(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_sa(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionR parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_1_src(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionI parse_instruction_i_branch(OpcodeI opcode, const std::string& instr);
        [[nodiscard]] InstructionI parse_instruction_i_branch_no_dst(OpcodeI opcode, const std::string& instr);
        [[nodiscard]] InstructionI parse_instruction_i_memory(OpcodeI opcode, const std::string& instr) const;
        [[nodiscard]] InstructionJ parse_instruction_j(OpcodeJ opcode, const std::string& instr);

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
        static [[nodiscard]] utils::regex_parser<Inner, Ts...> generate_parser(const std::string& pattern, std::string&& err_string = default_parser_err);
    };

    template <typename...Ts>
    utils::regex_parser<Parser::Inner, Ts...> Parser::generate_parser(const std::string& pattern, std::string&& err_string)
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
            else if (type == typeid(std::string)) p = "\\S+";
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
        return utils::regex_parser<Inner, Ts...>(std::move(reg), std::move(err_string));
    }
}
