#include <iostream>
#include <fstream>
#include <sstream>
#include "ASM/Compiler.hpp"
#include "Hardware/Processor.hpp"

using Hardware::WORD;
int main() {
	ASM::init();

	std::shared_ptr<ASM::Loader> loader{new ASM::Loader{".."}};
	ASM::Compiler compiler(loader);
	compiler.add_source(loader->get_id("code.txt"));

	Hardware::Computer computer(compiler.get_memory());

	auto execute_step = [&] {
		if(computer.processor->complete())
			return;

		auto segment = compiler.get_mapping(computer.processor->get_IP());
		auto code = loader->load(segment.source_id);
		std::cerr << "ASM code(source_id = " << segment.source_id << "): " << code.substr(segment.from, segment.to - segment.from) << "\n";

		computer.execute_step([&](WORD index) { return compiler.get_mapping(index); });
	};

	//while(!computer.processor->complete())
	//	execute_step();
	computer.execute(10000);
}
