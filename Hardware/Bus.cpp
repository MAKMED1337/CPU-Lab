#include "Bus.hpp"
#include <stdexcept>
#include <fmt/format.h>

namespace Hardware {
	WORD Bus::read(WORD address) const {
		auto& device = get_device(address);
		return device.read(address);
	}
	
	void Bus::write(WORD address, WORD value) {
		auto& device = get_device(address);
		device.write(address, value);
	}
	
	IMemory& Bus::get_device(WORD& address) const {
		if(CODE_OFFSET <= address && address < CODE_OFFSET + CODE_SIZE) {
			address -= CODE_OFFSET;
			return m_code;
		} else if(STACK_OFFSET <= address && address < STACK_OFFSET + STACK_SIZE) {
			address -= STACK_OFFSET;
			return m_stack;
		} else if(RAM_OFFSET <= address && address < RAM_OFFSET + RAM_SIZE) {
			address -= RAM_OFFSET;
			return m_ram;
		} else if(IO_OFFSET <= address && address < IO_OFFSET + IO_SIZE) {
			address -= IO_OFFSET;
			return m_IO;
		} else if(DISPLAY_OFFSET <= address && address < DISPLAY_OFFSET + DISPLAY_SIZE) {
			address -= DISPLAY_OFFSET;
			return m_display;
		}
		
		throw std::range_error(fmt::format("{} out of address space", address));
	}
	
	Bus::Bus(IMemory& code, IMemory& stack, IMemory& ram, IMemory& IO, IMemory& display) :
		m_code(code), m_stack(stack), m_ram(ram), m_IO(IO), m_display(display) {}
}