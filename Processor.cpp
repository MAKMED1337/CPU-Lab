#include "Processor.hpp"
#include <cassert>
#include <iostream>

void Processor::execute_step() {
	assert(!complete()); //FIXME: throw error
	
	auto command = memory[IP++];
	switch (command) {
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
	}
}



void Processor::dump(std::ostream& os) const {
	os << "A = " << A << " B = " << B << " IP = " << IP << "(" << (complete() ? "STOP" : keywords[memory[IP]]) << ") SP = " << SP << "\n";
}

bool Processor::complete() const {
	return !~IP;
}

void Processor::execute() {
	while(!complete()) {
		execute_step();
		dump(std::cerr);
	}
}
