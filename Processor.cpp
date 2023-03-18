#include "Processor.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <fmt/format.h>

namespace Hardware {
	void Processor::execute_step() {
		if (complete())
			throw std::runtime_error("Can't continue execution, because already completed");
		
		auto instruction = memory[IP++];
		using enum Instruction;
		switch (Instruction{ instruction }) {
			case READ:
				A = memory[A];
				break;
			case WRITE:
				memory[A] = B;
				break;
			case SWAP:
				std::swap(A, B);
				break;
			case SWAP_C:
				std::swap(A, C);
				break;
			case SET:
				A = memory[IP++];
				break;
			
			case JUMP_ZERO:
				IP = A ? IP + 1 : memory[IP];
				break;
			
			case READ_IP:
				A = IP;
				break;
			case WRITE_IP:
				IP = A;
				break;
			
			case READ_SP:
				A = SP;
				break;
			
			case WRITE_SP:
				SP = A;
				break;
			
			case ADD:
				A += B;
				break;
			case SUB:
				A -= B;
				break;
			case MULT:
				A *= B;
				break;
			case DIV:
				A /= B;
				break;
			
			case SHIFT_L:
				A <<= B;
				break;
			case SHIFT_R:
				A >>= B;
				break;
			case AND:
				A &= B;
				break;
			case OR:
				A |= B;
				break;
			case XOR:
				A ^= B;
				break;
			default:
				throw std::invalid_argument(fmt::format("Unknown instruction: {}", instruction));
		}
	}
	
	
	void Processor::dump(std::ostream& os) const {
		os << fmt::format("IP = {}({}) SP = {} A = {} B = {} C = {}\n", IP, complete() ? "COMPLETE" : names[memory[IP]], SP, A, B, C);
	}
	
	void Processor::dump_stack(std::ostream& os) const {
		os << "STACK:\n";
		for(WORD i = SP - 1; i >= STACK_OFFSET; --i)
			os << fmt::format("{}\n", memory[i]);
		os << "\n";
	}
	
	bool Processor::complete() const {
		return IP == STOP;
	}
	
	std::span<WORD> Processor::get_memory() {
		return std::span<WORD>{ memory };
	}
	
	std::span<WORD> Processor::get_code() {
		return get_memory().subspan(CODE_OFFSET, CODE_SIZE);
	}
	
	std::span<WORD> Processor::get_stack() {
		return get_memory().subspan(STACK_OFFSET, STACK_SIZE);
	}
	
	std::span<WORD> Processor::get_ram() {
		return get_memory().subspan(RAM_OFFSET, RAM_SIZE);
	}
	
	std::span<WORD> Processor::get_io() {
		return get_memory().subspan(IO_OFFSET, IO_SIZE);
	}
	
	std::span<WORD> Processor::get_display() {
		return get_memory().subspan(DISPLAY_OFFSET, DISPLAY_SIZE);
	}
};