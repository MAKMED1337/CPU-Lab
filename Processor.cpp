#include "Processor.hpp"
#include <cassert>
#include <iostream>

namespace Hardware {
	void Processor::execute_step() {
		assert(!complete()); //FIXME: throw error
		using
		enum Instruction;
		
		auto command = memory[IP++];
		switch (Instruction{ command }) {
			case READ:
				A = memory[A];
				break;
			case WRITE:
				memory[A] = B;
				break;
			case SWAP:
				std::swap(A, B);
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
				assert(false); //FIXME: throw error
		}
	}
	
	
	void Processor::dump(std::ostream& os) const {
		os << "A = " << A << " B = " << B << " IP = " << IP << "(" << (complete() ? "STOP" : names[memory[IP]])
		   << ") SP = " << SP << "\n";
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