#pragma once
#include "Memory.hpp"
#include "../ASM/CodeSegment.hpp"
#include "Bus.hpp"
#include "Processor.hpp"
#include <memory>

namespace Hardware {
	using ASM::CodeSegment;
	class Processor;
	class Computer final {
		std::unique_ptr<IMemory> m_code, m_stack, m_ram, m_io, m_display;
	public:
		Bus bus;
		std::unique_ptr<Processor> processor;
		Computer(std::unique_ptr<IMemory>&& code, std::unique_ptr<IMemory>&& stack, std::unique_ptr<IMemory>&& ram,
				 std::unique_ptr<IMemory>&& io, std::unique_ptr<IMemory>&& display);
		
		void execute(WORD limit);
		void execute_step(std::function<CodeSegment(WORD)> const& mapping);
	};
}