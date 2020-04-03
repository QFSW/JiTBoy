#pragma once
#include <cstdint>
#include <vector>
#include "x86_64.h"

enum class InstrMode
{
	RR,
	RM,
	MR,
	IR,
	IM
};

enum class JumpAdjust
{
	None,
	Always,
	Auto
};

class InstructionBuffer
{
public:
	void reset();
	void copy(void* ptr) const;
	[[nodiscard]] size_t size() const noexcept;

	#pragma region Non Immediate Instructions
	
	template <Opcode Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register dst, Register src);

	template <Opcode Op, InstrMode Mode, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register dst, Register src, int32_t addr_offset);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register reg);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register reg, int32_t addr_offset);

	template <Opcode Op>
	void instr() { write_raw(Op); }
	
	#pragma endregion 

	#pragma region Immediate Instructions
	
	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::IR, RegisterSize Size = RegisterSize::Reg32>
	void instr_imm(Register dst, uint32_t imm);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::IR, RegisterSize Size = RegisterSize::Reg32>
	void instr_imm(Register dst, uint32_t imm, int32_t addr_offset);

	#pragma endregion

	#pragma region Jump Instructions

	template <JumpAdjust Adjust = JumpAdjust::Auto>
	void jump(int32_t offset);

	template <CondCode Cond, JumpAdjust Adjust = JumpAdjust::Auto>
	void jump_cond(int32_t offset);

	template <JumpAdjust Adjust = JumpAdjust::Auto>
	void call(int32_t offset);
	
	#pragma endregion

	#pragma region Misc Intructions

	template <CondCode Cond, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void move_cond(Register dst, Register src);

	template <CondCode Cond, InstrMode Mode, RegisterSize Size = RegisterSize::Reg32>
	void move_cond(Register dst, Register src, int32_t addr_offset);

	void bswap(Register dst);
	void enter(uint16_t size, uint8_t nesting = 0);
	
	#pragma endregion 

private:
	std::vector<uint8_t> _buffer;

	template <typename T, int Size = sizeof(T)>
	void write_raw(T data);

	void write_raw(uint8_t data);

	template <Opcode Op, InstrMode Mode, RegisterSize Size>
	static constexpr uint8_t encode_opcode();

	template <Opcode Op, RegisterSize Size>
	static constexpr uint8_t encode_imm_opcode(uint32_t imm);

	template <Opcode Op, InstrMode Mode, RegisterSize Size>
	void write_opcode();

	template <Opcode Op, RegisterSize Size>
	void write_imm_opcode(uint32_t imm);

	template <Opcode Op, RegisterSize Size>
	void write_immediate(uint32_t imm);

	void encode_regs(RegisterMode mode, Register dst, Register src);
	void encode_regs_offset(Register dst, Register src, int32_t addr_offset);

	template <JumpAdjust Adjust>
	static constexpr int32_t adjust_offset(int32_t offset, const uint8_t instr_size);

	template <Opcode Op>
	static constexpr bool is_fast_imm_instr();

	template <JumpAdjust Adjust, uint8_t NearSize = 2>
	static constexpr bool is_near_jump(int32_t offset);

	static constexpr bool is_8_bit(int32_t val);
};

#pragma region Non Immediate Instruction Implementations

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register dst, const Register src)
{
	write_opcode<Op, Mode, Size>();
	
	if constexpr (Mode == InstrMode::RR) encode_regs(RegisterMode::Register, dst, src);
	else if constexpr (Mode == InstrMode::RM) encode_regs(RegisterMode::MemoryDisp0, dst, src);
	else if constexpr (Mode == InstrMode::MR) encode_regs(RegisterMode::MemoryDisp0, src, dst);
	else throw "Invalid instruction mode encountered";
}

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register dst, const Register src, const int32_t addr_offset)
{
	write_opcode<Op, Mode, Size>();
	
	if constexpr (Mode == InstrMode::RM) encode_regs_offset(dst, src, addr_offset);
	else if constexpr (Mode == InstrMode::MR) encode_regs_offset(src, dst, addr_offset);
	else throw "Invalid instruction mode encountered";
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register reg)
{
	instr<Op, Mode, Size>(reg, static_cast<Register>(Ext));
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register reg, const int32_t addr_offset)
{
	instr<Op, Mode, Size>(reg, static_cast<Register>(Ext), addr_offset);
}

#pragma endregion

#pragma region Immediate Instruction Implementations

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr_imm(const Register dst, const uint32_t imm)
{
	write_imm_opcode<Op, Size>(imm);

	if constexpr (Mode == InstrMode::IR) encode_regs(RegisterMode::Register, dst, static_cast<Register>(Ext));
	else if constexpr (Mode == InstrMode::IM) encode_regs(RegisterMode::MemoryDisp0, dst, static_cast<Register>(Ext));
	else throw "Invalid instruction mode encountered";

	write_immediate<Op, Size>(imm);
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr_imm(const Register dst, const uint32_t imm, const int32_t addr_offset)
{
	write_imm_opcode<Op, Size>(imm);

	if constexpr (Mode == InstrMode::IM) encode_regs_offset(dst, static_cast<Register>(Ext), addr_offset);
	else throw "Invalid instruction mode encountered";

	write_immediate<Op, Size>(imm);
}

#pragma endregion 

#pragma region Jump Instruction Implementations

template <JumpAdjust Adjust>
constexpr int32_t InstructionBuffer::adjust_offset(const int32_t offset, const uint8_t instr_size)
{
	if constexpr (Adjust == JumpAdjust::Always) return offset - instr_size;
	else if constexpr (Adjust == JumpAdjust::Auto)
	{
		if (offset < 0) return offset - instr_size;
	}

	return offset;
}

template <JumpAdjust Adjust>
void InstructionBuffer::jump(int32_t offset)
{
	const bool is_near = is_near_jump<Adjust>(offset);
	const uint8_t instr_size = 1 + (is_near ? 1 : 4);
	
	offset = adjust_offset<Adjust>(offset, instr_size);

	if (is_near)
	{
		write_raw(JMP_8);
		write_raw<int8_t>(offset);
	}
	else
	{
		write_raw(JMP_32);
		write_raw<int32_t>(offset);
	}
}

template <CondCode Cond, JumpAdjust Adjust>
void InstructionBuffer::jump_cond(int32_t offset)
{
	const bool is_near = is_near_jump<Adjust>(offset);
	const uint8_t instr_size = 1 + (is_near ? 1 : 5);

	offset = adjust_offset<Adjust>(offset, instr_size);

	if (is_near)
	{
		constexpr uint8_t op = static_cast<uint8_t>(Cond) | static_cast<uint8_t>(Jcc_8);
		
		write_raw(op);
		write_raw<int8_t>(offset);
	}
	else
	{
		constexpr uint8_t op = static_cast<uint8_t>(Cond) | static_cast<uint8_t>(Jcc_32);
		
		write_raw(OpcodePrefix::Jcc_32);
		write_raw(op);
		write_raw<int32_t>(offset);
	}
}

template <JumpAdjust Adjust>
void InstructionBuffer::call(int32_t offset)
{
	offset = adjust_offset<Adjust>(offset, 5);

	write_raw(CALL);
	write_raw<int32_t>(offset);
}

#pragma endregion 

#pragma region Misc Instruction Implementations

template <CondCode Cond, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::move_cond(const Register dst, const Register src)
{
	constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(CMOVcc) | static_cast<uint8_t>(Cond));

	if constexpr (Size == RegisterSize::Reg8) throw "CMOVcc does not support 8bit operands";
	else if constexpr (Size == RegisterSize::Reg16) write_raw(OpcodePrefix::Size16);

	write_raw(OpcodePrefix::CMOVcc);

	if constexpr (Mode == InstrMode::RR) instr<op, InstrMode::RR, RegisterSize::Reg8>(src, dst);
	else if constexpr (Mode == InstrMode::MR) instr<op, InstrMode::RM, RegisterSize::Reg8>(src, dst);
	else throw "Unsupported instruction mode";
}

template <CondCode Cond, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::move_cond(const Register dst, const Register src, const int32_t addr_offset)
{
	constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(CMOVcc) | static_cast<uint8_t>(Cond));

	if constexpr (Size == RegisterSize::Reg8) throw "CMOVcc does not support 8bit operands";
	else if constexpr (Size == RegisterSize::Reg16) write_raw(OpcodePrefix::Size16);

	write_raw(OpcodePrefix::CMOVcc);

	if constexpr (Mode == InstrMode::MR) instr<op, InstrMode::RM, RegisterSize::Reg8>(src, dst, addr_offset);
	else throw "Unsupported instruction mode";
}

#pragma endregion

template <typename T, int Size>
void InstructionBuffer::write_raw(const T data)
{
	if constexpr (Size == 1)
	{
		_buffer.push_back(static_cast<uint8_t>(data));
	}
	else
	{
		_buffer.resize(size() + Size);
		if constexpr (Size == sizeof(T))
		{
			T* buffer_ptr = reinterpret_cast<T*>(&_buffer.back() - Size + 1);
			*buffer_ptr = data;
		}
		else
		{
			void* buffer_ptr = &_buffer.back() - Size + 1;
			memcpy(buffer_ptr, &data, Size);
		}
	}
}

template <Opcode Op, InstrMode Mode, RegisterSize Size>
constexpr uint8_t InstructionBuffer::encode_opcode()
{
	auto op = static_cast<uint8_t>(Op);
	switch (Size)
	{
		case RegisterSize::Reg16:
		case RegisterSize::Reg32: op |= 0b1; break;
		default: break;
	}

	switch (Mode)
	{
		case InstrMode::MR: op |= 0b10; break;
		default: break;
	}

	return op;
}

template <Opcode Op, RegisterSize Size>
constexpr uint8_t InstructionBuffer::encode_imm_opcode(const uint32_t imm)
{
	auto opcode = static_cast<uint8_t>(Op);
	if constexpr (Size == RegisterSize::Reg8)
	{
		return opcode;
	}
	else
	{
		opcode |= 0b1;
		if constexpr (is_fast_imm_instr<Op>())
		{
			if (is_8_bit(imm))
			{
				opcode |= 0b10;
			}
		}

		return opcode;
	}
}

template <Opcode Op>
constexpr bool InstructionBuffer::is_fast_imm_instr()
{
	switch (Op)
	{
	case MOV_I:
	case CALL:
	case RET:
		return false;
	default:
		return true;
	}
}

template <JumpAdjust Adjust, uint8_t NearSize>
constexpr bool InstructionBuffer::is_near_jump(const int32_t offset)
{
	if constexpr (Adjust == JumpAdjust::None)
	{
		return is_8_bit(offset);
	}
	else if constexpr (Adjust == JumpAdjust::Auto)
	{
		return offset >= -128 + NearSize && offset <= 127;
	}
	else
	{
		return offset >= -128 + NearSize && offset <= 127 + NearSize;
	}
}

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::write_opcode()
{
	constexpr uint8_t opcode = encode_opcode<Op, Mode, Size>();
	if constexpr (Size == RegisterSize::Reg16)
	{
		write_raw(OpcodePrefix::Size16);
	}

	write_raw(opcode);
}

template <Opcode Op, RegisterSize Size>
void InstructionBuffer::write_imm_opcode(const uint32_t imm)
{
	const uint8_t opcode = encode_imm_opcode<Op, Size>(imm);
	if constexpr (Size == RegisterSize::Reg16)
	{
		write_raw(OpcodePrefix::Size16);
	}

	write_raw(opcode);
}

template <Opcode Op, RegisterSize Size>
void InstructionBuffer::write_immediate(const uint32_t imm)
{
	if constexpr (Size == RegisterSize::Reg8) write_raw<uint8_t>(imm);
	else
	{
		constexpr bool fast = is_fast_imm_instr<Op>();
		if (fast && is_8_bit(imm)) write_raw<uint8_t>(imm);
		else if constexpr (Size == RegisterSize::Reg16) write_raw<uint16_t>(imm);
		else if constexpr (Size == RegisterSize::Reg32) write_raw<uint32_t>(imm);
		else throw "Unsupported register size";
	}
}

