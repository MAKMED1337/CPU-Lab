#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#include <stdexcept>
#include <fmt/format.h>
#include <iostream>
#include "../foreach_macro.hpp"
#include "Memory.hpp"
#include "Computer.hpp"

#define DECLARE(x, y) x,
#define NAME(x, y) #x,
#define PLUS_ONE(x, y) + 1
#define ENUM_CASE(name, code) case name: code; break;
#define MAKE_INSTRUCTIONS(...)                                                                \
enum class Instruction : WORD {                                                               \
	FOR_EACH2(DECLARE, __VA_ARGS__)                                                           \
};                                                                                            \
                                                                                              \
static inline std::array<std::string_view, FOR_EACH2(PLUS_ONE, __VA_ARGS__)> names{{          \
    FOR_EACH2(NAME, __VA_ARGS__)                                                              \
}};                                                                                           \
                                                                                              \
void execute_step() {                                                                         \
	if (complete())                                                                           \
		throw std::runtime_error("Can't continue execution, because already completed");      \
                                                                                              \
	auto instruction = bus().read(IP++);                                                      \
	using enum Instruction;                                                                   \
	switch (Instruction{ instruction }) {                                                     \
        FOR_EACH2(ENUM_CASE, __VA_ARGS__)                                                     \
        default:                                                                              \
			throw std::invalid_argument(fmt::format("Unknown instruction: {}", instruction)); \
    };                                                                                        \
}

namespace Hardware {
	enum class Instruction : WORD;
	
	class Computer;
	class Processor final {
		Computer& m_computer;
		
		//To avoid incomplete type error
		Bus& bus();
	public:
		//registers SP(STACK POINTER), IP(INSTRUCTION POINTER), A, B, C
		WORD IP = 0, SP = STACK_OFFSET, A = 0, B = 0, C = 0;
		
		//To stop, set IP to STOP
		static constexpr WORD STOP = ~WORD{ 0 };
		
		MAKE_INSTRUCTIONS(
			READ, A = bus().read(A),                     // A <- mem[A]
			WRITE, bus().write(A, B),                    // mem[A] <- B
			SWAP, std::swap(A, B),                              // A <-> B
			SWAP_C, std::swap(A, C),                            // A <-> C
			SET, A = bus().read(IP++),                   // A <- mem[IP + 1], IP = IP + 2
			
			JUMP_ZERO, IP = A ? IP + 1 : bus().read(IP), // if A == 0 then IP = argument
			
			READ_IP, A = IP,                                    // A <- IP + 1(next instruction address)
			WRITE_IP, IP = A,                                   // IP <- A
			READ_SP, A = SP,                                    // A <- SP
			WRITE_SP, SP = A,                                   // SP <- A
			
			ADD, A += B,                                        // A += B
			SUB, A -= B,                                        // A -= B
			MULT, A *= B,                                       // A *= B
			DIV, A /= B,                                        // A /= B (floor div)
			MOD, A %= B,                                        // A %= B
			
			SHIFT_L, A <<= B,                                   // A <<= B
			SHIFT_R, A >>= B,                                   // A >>= B
			AND, A &= B,                                        // A &= B
			OR, A |= B,                                         // A |= B
			XOR, A ^= B,                                        // A ^= B
			NOT, A = ~A,                                        // A = ~A
			
			LNOT, A = !A,                                       // A = !A
			
			DUMP, dump(std::cout),                              //dumps processor state
		)
		
		Processor(Computer& computer) : m_computer(computer) {}
		
		bool complete() const;
		
		void dump_registers(std::ostream& os) const;
		void dump_stack(std::ostream& os) const;
		void dump(std::ostream& os) const;
	};
}