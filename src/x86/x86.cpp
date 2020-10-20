#include "x86.hpp"

namespace x86
{
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
				case OpcodeExt::ADD_I: return "ADD";
				case OpcodeExt::AND_I: return "AND";
				default: return "???";
			}
			case SAL_I: switch (ext)
			{
				case OpcodeExt::SAL_I: return "SAL";
				case OpcodeExt::SAR_I: return "SAR";
				case OpcodeExt::SHR_I: return "SHR";
				default: return "???";
			}
			default: return "???";
		}
	}

	const char* reg_to_string(const Register reg, const RegisterSize size)
	{
		switch (size)
		{
			case RegisterSize::Reg8: switch (reg)
			{
				case Register::AL: return "AL";
				case Register::CL: return "CL";
				case Register::DL: return "DL";
				case Register::BL: return "BL";
				case Register::AH: return "AH";
				case Register::CH: return "CH";
				case Register::DH: return "DH";
				case Register::BH: return "BH";
				default: return "???";
			}
			case RegisterSize::Reg16: switch (reg)
			{
				case Register::AX: return "AX";
				case Register::CX: return "CX";
				case Register::DX: return "DX";
				case Register::BX: return "BX";
				case Register::SP: return "SP";
				case Register::BP: return "BP";
				case Register::SI: return "SI";
				case Register::DI: return "DI";
				default: return "???";
			}
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
			}
			case RegisterSize::Reg64: switch (reg)
			{
				case Register::RAX: return "RAX";
				case Register::RCX: return "RCX";
				case Register::RDX: return "RDX";
				case Register::RBX: return "RBX";
				case Register::RSP: return "RSP";
				case Register::RBP: return "RBP";
				case Register::RSI: return "RSI";
				case Register::RDI: return "RDI";
				case Register::R8: return "R8";
				case Register::R9: return "R9";
				case Register::R10: return "R10";
				case Register::R11: return "R11";
				case Register::R12: return "R12";
				case Register::R13: return "R13";
				case Register::R14: return "R14";
				case Register::R15: return "R15";
				default: return "???";
			}
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
}