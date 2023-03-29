#include "Code.hpp"
#include <stdexcept>

namespace Hardware {
	Code::Code(std::array<WORD, CODE_SIZE> code) : memory(code) {}
	
	WORD Code::read(WORD address) const {
		return memory.at(address);
	}
	
	void Code::write(Hardware::WORD address, Hardware::WORD value) {
		throw std::invalid_argument("Code section is unwritable"); //TODO: rethink if this necessary
	}
}