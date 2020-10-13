#include "x86_64.hpp"

const char* opcode_to_string(const Opcode op)
{
	switch (op)
	{
		case ADD: return "ADD";
		case ADC: return "ADC";
		case SUB: return "SUB";
		case MOV: return "MOV";
		case INC: return "INC";
		case NEG: return "NEG";
		case CMP: return "CMP";
		case AND: return "AND";
		case OR: return "OR";
		case XOR: return "XOR";
		case PUSH: return "PUSH";
		case POP: return "POP";
		case NOP: return "NOP";
		case RET: return "RET";
		case ENTER: return "ENTER";
		case LEAVE: return "LEAVE";
		case CALL: return "CALL";
		default: return "???";
	}
}

const char* opcode_imm_to_string(const Opcode op, const OpcodeExt ext)
{
	switch (op)
	{
		case MOV_I: switch (ext)
		{
			case OpcodeExt::MOV_I: return "MOV";
			default: return "???";
		}
		case CMP_I: switch (ext)
		{
			case OpcodeExt::CMP_I: return "CMP";
			default: return "???";
		}
		default: return "???";
	}
}

const char* reg_to_string(const Register reg, const RegisterSize size)
{
	switch (size)
	{
		case RegisterSize::Reg8: return "???";
		case RegisterSize::Reg16: return "???";
		case RegisterSize::Reg32: switch (reg)
		{

			case Register::EAX: return "EAX";
			case Register::ECX: return "ECX";
			case Register::EDX: return "EDX";
			case Register::EBX: return "EBX";
			case Register::ESP: return "ESP";
			case Register::EBP: return "EBP";
			case Register::ESI: return "ESI";
			case Register::EDI: return "EDI";
			default: return "???";
		};
		default: return "???";
	}
}

const char* cond_to_string(const CondCode cond)
{
	switch (cond)
	{
		case CondCode::A: return "A";
		case CondCode::AE: return "AE";
		case CondCode::B: return "B";
		case CondCode::BE: return "BE";
		case CondCode::E: return "E";
		case CondCode::G: return "G";
		case CondCode::GE: return "GE";
		case CondCode::L: return "L";
		case CondCode::LE: return "LE";
		case CondCode::NE: return "NE";
		case CondCode::NO: return "NO";
		case CondCode::NP: return "NP";
		case CondCode::NS: return "NS";
		case CondCode::O: return "O";
		case CondCode::P: return "P";
		case CondCode::S: return "S";
		default: return "???";
	}
}