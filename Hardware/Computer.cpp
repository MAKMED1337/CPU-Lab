#include "Computer.hpp"

namespace Hardware {
	Computer::Computer(std::array<WORD, CODE_SIZE> OS) :
		code(OS), bus(code, stack, ram, io, display), processor{new Processor{*this}} {}
		
	void Computer::run(WORD limit) {
		for (WORD i = 0; i < limit && !processor->complete(); ++i) {
			processor->execute_step();
		}
	}
}