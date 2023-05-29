#include "IO.hpp"
#include <cassert>
#include <stdexcept>
#include <fmt/format.h>
#include <iostream>

namespace Hardware {
	WORD IO::read(WORD address) const {
		if (address == 1) {
			char c;
			std::cin >> c;
			return c;
		}
		throw std::invalid_argument(fmt::format("IO address(relative) {} is unreadable", address));
	}
	
	void IO::write(WORD address, WORD value) {
		if (address == 0) {
			if (value > CHAR_MAX) {
				throw std::invalid_argument(fmt::format("Value {} too big to be printed", value));
			}
			std::cout << (char)value;
			return;
		}
		
		throw std::invalid_argument(fmt::format("IO address(relative) {} is unwritable", address));
	}
}