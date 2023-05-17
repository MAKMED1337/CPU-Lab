#include <iostream>
#include <fstream>
#include <sstream>
#include "ASM/Compiler.hpp"
#include "Hardware/Processor.hpp"

int main() {
	ASM::init();

	std::ifstream source_file("../code.txt"); //because .exe file in cmake-build-... folder
	std::ostringstream buffer;
	buffer << source_file.rdbuf();
	std::string code = buffer.str();
	
	ASM::Compiler compiler;
	compiler.append(code);

	Hardware::Computer computer(compiler.get_memory());

	computer.execute_step(compiler.get_mapping());
	auto segment = compiler.get_mapping()[computer.processor->get_IP()];
	std::cerr << "ASM code: " << code.substr(segment.from, segment.to - segment.from) << "\n";
	//need to be second instruction, because first already done. (SET 42069)
}
