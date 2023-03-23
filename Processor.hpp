#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#include <span>
#include <stdexcept>
#include <fmt/format.h>
#include <iostream>
#include "macros.hpp"

#define DECLARE(x, y) x,
#define NAME(x, y) #x,
#define PLUS_ONE(x, y) + 1
#define ENUM_CASE(name, code) case name: code; break;
#define MAKE_INSTRUCTIONS(...)                                                                \
enum class Instruction : WORD {                                                               \
	FOR_EACH2(DECLARE, __VA_ARGS__)                                                           \
};                                                                                            \
                                                                                              \
std::array<std::string_view, FOR_EACH2(PLUS_ONE, __VA_ARGS__)> names{{                        \
    FOR_EACH2(NAME, __VA_ARGS__)                                                              \
}};                                                                                           \
                                                                                              \
void execute_step() {                                                                         \
	if (complete())                                                                           \
		throw std::runtime_error("Can't continue execution, because already completed");      \
                                                                                              \
	auto instruction = memory[IP++];                                                          \
	using enum Instruction;                                                                   \
	switch (Instruction{ instruction }) {                                                     \
        FOR_EACH2(ENUM_CASE, __VA_ARGS__)                                                     \
        default:                                                                              \
			throw std::invalid_argument(fmt::format("Unknown instruction: {}", instruction)); \
    };                                                                                        \
}

namespace Hardware {
	using WORD = uint32_t;
	using DWORD = uint64_t; //For overflow's checking, not used in processor
	
	constexpr WORD CODE_SIZE = 8 * 1024 * 1024, STACK_SIZE = 8 * 1024 * 1024, RAM_SIZE = 8 * 1024 * 1024,
		IO_SIZE = 128, DISPLAY_SIZE = 1920 * 1080,
		MEMORY_SIZE = CODE_SIZE + STACK_SIZE + RAM_SIZE + IO_SIZE + DISPLAY_SIZE;
	
	constexpr WORD CODE_OFFSET = 0, STACK_OFFSET = CODE_OFFSET + CODE_SIZE, RAM_OFFSET = STACK_OFFSET + STACK_SIZE,
		IO_OFFSET = RAM_OFFSET + RAM_SIZE, DISPLAY_OFFSET = IO_OFFSET + IO_SIZE;
	
	using memory_t = std::array<WORD, MEMORY_SIZE>;

	//To stop, set IP to STOP
	struct Processor final {
		memory_t memory;
		WORD IP = 0, SP = STACK_OFFSET, A = 0, B = 0, C = 0;
		static constexpr WORD STOP = ~WORD{ 0 };
		
		MAKE_INSTRUCTIONS(
			READ, A = memory[A],                          // A <- mem[A]
			WRITE, memory[A] = B,                         // mem[A] <- B
			SWAP, std::swap(A, B),                        // A <-> B
			SWAP_C, std::swap(A, C),                      // A <-> C
			SET, A = memory[IP++],                        // A <- mem[IP + 1], IP = IP + 2
			
			JUMP_ZERO, IP = A ? IP + 1 : memory[IP],      // if A == 0 then IP = argument
			
			READ_IP, A = IP,                              // A <- IP + 1(next instruction address)
			WRITE_IP, IP = A,                             // IP <- A
			READ_SP, A = SP,                              // A <- SP
			WRITE_SP, SP = A,                             // SP <- A
			
			ADD, A += B,                                  // A += B
			SUB, A -= B,                                  // A -= B
			MULT, A *= B,                                 // A *= B
			DIV, A /= B,                                  // A /= B (floor div)
			MOD, A %= B,                                  // A %= B
			
			SHIFT_L, A <<= B,                             // A <<= B
			SHIFT_R, A >>= B,                             // A >>= B
			AND, A &= B,                                  // A &= B
			OR, A |= B,                                   // A |= B
			XOR, A ^= B,                                  // A ^= B
			NOT, A = ~A,                                  // A = ~A
			
			LNOT, A = !A,                                 // A = !A
			
			DUMP, dump(std::cout),                        //dumps processor state
		)
		
		Processor(memory_t&& code) : memory(std::move(code)) {}
		
		bool complete() const;
		
		void dump_registers(std::ostream& os) const;
		void dump_stack(std::ostream& os) const;
		void dump(std::ostream& os) const;
		
		std::span<WORD> get_memory();
		std::span<WORD> get_code();
		std::span<WORD> get_stack();
		std::span<WORD> get_ram();
		std::span<WORD> get_io();
		std::span<WORD> get_display();
	};
};