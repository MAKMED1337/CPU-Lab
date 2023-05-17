#pragma once
#include "Code.hpp"
#include "Stack.hpp"
#include "RAM.hpp"
#include "IO.hpp"
#include "Display.hpp"
#include "Bus.hpp"
#include "Processor.hpp"
#include "../ASM/CodeSegment.hpp"

namespace Hardware {
	using ASM::CodeSegment;
	class Processor;
	class Computer final {
		Code code;
		Stack stack;
		RAM ram;
		IO io;
		Display display;
	public:
		Bus bus;
		std::unique_ptr<Processor> processor;
		Computer(std::array<WORD, CODE_SIZE> OS);
		
		void execute(WORD limit);
		void execute_step(std::array<CodeSegment, Hardware::CODE_SIZE> const& instructions_mapping);
	};
}