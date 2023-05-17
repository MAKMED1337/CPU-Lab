#include "Computer.hpp"

namespace Hardware {
	Computer::Computer(std::array<WORD, CODE_SIZE> OS) :
		code(OS), bus(code, stack, ram, io, display), processor{new Processor{*this}} {}
		
	void Computer::execute(WORD limit) {
		for (WORD i = 0; i < limit && !processor->complete(); ++i) {
			processor->execute_step();
		}
	}

	void Computer::execute_step(std::function<CodeSegment(WORD)> const& mapping) {
		auto start_segment = mapping(processor->get_IP());
		while(!processor->complete()) {
			auto segment = mapping(processor->get_IP());
			if (segment != start_segment)
				break;
			processor->execute_step();
		}
	}
}