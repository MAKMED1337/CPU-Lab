#include "Computer.hpp"

namespace Hardware {
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

	Computer::Computer(std::unique_ptr<IMemory> &&code, std::unique_ptr<IMemory> &&stack, std::unique_ptr<IMemory> &&ram,
					   std::unique_ptr<IMemory> &&io, std::unique_ptr<IMemory> &&display) :
		m_code(std::move(code)), m_stack(std::move(stack)), m_ram(std::move(ram)), m_io(std::move(io)),
		m_display(std::move(display)),
		bus(*m_code, *m_stack, *m_ram, *m_io, *m_display), processor{new Processor{*this}} {}
}