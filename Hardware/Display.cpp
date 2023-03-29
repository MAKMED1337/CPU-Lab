#include "Display.hpp"
#include <stdexcept>
#include <fmt/format.h>

namespace Hardware {
	WORD Display::read(WORD address) const {
		throw std::invalid_argument(fmt::format("Display address(relative) {} is unreadable", address));
	}
	
	void Display::write(WORD address, WORD value) {
		throw std::invalid_argument(fmt::format("Display address(relative) {} is unwritable", address));
	}
}