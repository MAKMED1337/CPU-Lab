#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#include <span>

namespace Hardware {
	using WORD = uint32_t;
	using DWORD = uint64_t; //For overflow's checking, not used in processor
	
	enum class Instruction : WORD {
		READ,                        // A <- mem[A]
		WRITE,                       // mem[A] <- B
		SWAP,                        // A <-> B
		SET,                         // A <- mem[IP + 1], IP = IP + 2
		
		JUMP_ZERO,                   // if A == 0 then IP = argument
		
		READ_IP,                     // A <- IP + 1(next instruction address)
		WRITE_IP,                    // IP <- A
		
		READ_SP,                     // A <- SP
		WRITE_SP,                    // SP <- A
		
		ADD,                         // A += B
		SUB,                         // A -= B
		MULT,                        // A *= B
		DIV,                         // A /= B
		
		SHIFT_L,                     // A <<= B
		SHIFT_R,                     // A >>= B
		AND,                         // A &= B
		OR,                          // A |= B
		XOR,                         // A ^= B
	};
	
	constexpr std::array<std::string_view, 18> names = {{
	   "READ", "WRITE", "SWAP", "SET", "JUMP_ZERO", "READ_IP","WRITE_IP", "READ_SP", "WRITE_SP", "ADD", "SUB", "MULT",
	   "DIV", "SHIFT_L", "SHIFT_R", "AND", "OR", "XOR"
	}};
	
	constexpr WORD CODE_SIZE = 8 * 1024 * 1024, STACK_SIZE = 8 * 1024 * 1024, RAM_SIZE = 8 * 1024 * 1024,
		IO_SIZE = 128, DISPLAY_SIZE = 1920 * 1080,
		MEMORY_SIZE = CODE_SIZE + STACK_SIZE + RAM_SIZE + IO_SIZE + DISPLAY_SIZE;
	
	constexpr WORD CODE_OFFSET = 0, STACK_OFFSET = CODE_OFFSET + CODE_SIZE, RAM_OFFSET = STACK_OFFSET + STACK_SIZE,
		IO_OFFSET = RAM_OFFSET + RAM_SIZE, DISPLAY_OFFSET = IO_OFFSET + IO_SIZE;
	
	using memory_t = std::array<WORD, MEMORY_SIZE>;

	//To stop, set IP to STOP
	struct Processor final {
		memory_t memory;
		WORD A = 0, B = 0, IP = 0, SP = CODE_SIZE;
		static constexpr WORD STOP = ~WORD{ 0 };
		
		Processor(memory_t&& code) : memory(std::move(code)) {}
		
		void execute_step();
		
		bool complete() const;
		
		void dump(std::ostream& os) const;
		
		std::span<WORD> get_memory();
		std::span<WORD> get_code();
		std::span<WORD> get_stack();
		std::span<WORD> get_ram();
		std::span<WORD> get_io();
		std::span<WORD> get_display();
	};
};