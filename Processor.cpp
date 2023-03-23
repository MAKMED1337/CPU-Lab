#include "Processor.hpp"
#include <iostream>
#include <stdexcept>
#include <fmt/format.h>

namespace Hardware {
	void Processor::dump_registers(std::ostream& os) const {
		os << fmt::format("IP = {}({}) SP = {} A = {} B = {} C = {}\n", IP, complete() ? "COMPLETE" : names[memory[IP]], SP, A, B, C);
	}
	
	void Processor::dump_stack(std::ostream& os) const {
		os << "STACK:\n";
		for(WORD i = SP; i >= STACK_OFFSET; --i)
			os << fmt::format("{}\n", memory[i]);
		os << "\n";
	}
	
	void Processor::dump(std::ostream& os) const {
		dump_registers(os);
		dump_stack(os);
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