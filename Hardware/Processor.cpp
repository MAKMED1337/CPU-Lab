#include "Processor.hpp"
#include <iostream>
#include <stdexcept>
#include <fmt/format.h>

namespace Hardware {
Hardware::Bus& Hardware::Processor::bus() {
	return m_computer.bus;
}

void Processor::dump_registers(std::ostream& os) const {
		os << fmt::format("IP = {}({}) SP = {} A = {} B = {} C = {}\n", IP, complete() ? "COMPLETE" : names[m_computer.bus.read(IP)], SP, A, B, C);
	}
	
	void Processor::dump_stack(std::ostream& os) const {
		os << "STACK:\n";
		for(WORD i = SP; i >= STACK_OFFSET; --i)
			os << fmt::format("{}\n", m_computer.bus.read(i));
		os << "\n";
	}
	
	void Processor::dump(std::ostream& os) const {
		dump_registers(os);
		dump_stack(os);
	}
	
	bool Processor::complete() const {
		return IP == STOP;
	}
};