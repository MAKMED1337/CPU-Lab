#pragma once
#include <array>
#include <cstdint>
#include <string_view>

using WORD = uint32_t;
using DWORD = uint64_t;

enum Instruction : WORD {
	READ,						// A <- mem[A]
	WRITE,						// mem[A] <- B
	SWAP,						// A <-> B
	SET,						// A <- mem[IP + 1], IP = IP + 2
	
	JUMP_ZERO,					// if A == 0 then IP = argument
	
	READ_IP,					// A <- IP + 1(next instruction address)
	WRITE_IP,					// IP <- A
	
	READ_SP,					// A <- SP
	WRITE_SP,					// SP <- A
	
	ADD,						// A += B
	SUB,						// A -= B
	MULT,						// A *= B
	DIV,						// A /= B
	
	SHIFT_L,					// A <<= B
	SHIFT_R,					// A >>= B
	AND,						// A &= B
	OR,							// A |= B
	XOR,						// A ^= B
	
	//<--- 18
};

constexpr std::array<std::string_view, 18> keywords = {{
   "READ", "WRITE", "SWAP", "SET", "JUMP_ZERO", "READ_IP","WRITE_IP", "READ_SP", "WRITE_SP", "ADD", "SUB", "MULT",
   "DIV", "SHIFT_L", "SHIFT_R", "AND", "OR", "XOR"
}};

constexpr WORD MEMORY_SIZE = 32 * 1024 * 1024;
using memory_t = std::array<WORD, MEMORY_SIZE>;


//To stop, set IP to ~0 address
class Processor final {
	memory_t memory;
	WORD A = 0, B = 0, IP = 0, SP = CODE_SIZE;
public:
	Processor(memory_t&& code) : memory(std::move(code)) {}
	
	void execute_step();
	void execute();
	
	bool complete() const;
	
	void dump(std::ostream& os) const;
	static constexpr WORD CODE_SIZE = 8 * 1024 * 1024, STACK_SIZE = 8  * 1024 * 1024;
};
