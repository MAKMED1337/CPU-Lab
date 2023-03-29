#pragma once
#include "Code.hpp"
#include "Stack.hpp"
#include "RAM.hpp"
#include "IO.hpp"
#include "Display.hpp"
#include "Bus.hpp"
#include "Processor.hpp"

namespace Hardware {
	class Processor;
	class Computer final {
		Code code;
		Stack stack;
		RAM ram;
		IO io;
		Display display;
		
		std::unique_ptr<Processor> processor;
	public:
		Bus bus;
		Computer(std::array<WORD, CODE_SIZE> OS);
		
		void run(WORD limit);
	};
}