#pragma once
#include "Code.hpp"
#include "Stack.hpp"
#include "RAM.hpp"
#include "IO.hpp"
#include "Display.hpp"

namespace Hardware {
	class Bus final : public IMemory {
		IMemory &m_code, &m_stack, &m_ram, &m_IO, &m_display;
		
		//changes address to relative
		IMemory& get_device(WORD& address) const;
	public:
		Bus(IMemory& code, IMemory& stack, IMemory& ram, IMemory& IO, IMemory &display);
		
		WORD read(WORD address) const override;
		void write(WORD address, WORD value) override;
	};
}