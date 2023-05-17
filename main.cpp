#include <iostream>
#include <fstream>
#include "ASM/Compiler.hpp"
#include "Hardware/Processor.hpp"
#include "Hardware/InteractiveCode.hpp"
#include "Hardware/Code.hpp"

using Hardware::WORD;
int main(int argc, const char* argv[]) {
	assert(argc == 2 && "-i or --interactive or code file location");
	ASM::init();

	std::unique_ptr<Hardware::IMemory> code;

	std::shared_ptr<ASM::Loader> loader{new ASM::Loader{""}};
	ASM::Compiler compiler(loader);
	if(std::string{argv[1]} == "-i" || std::string{argv[1]} == "--interactive") { //TODO: made more accurate
		auto interactive_shell = [&]() {
			std::cout << "enter code:\n";
			std::string asm_code, line;
			while(std::getline(std::cin, line)) {
				if(line == "--end")
					break;
				asm_code += line + "\n";
			}

			auto offset = compiler.offset();
			compiler.add_source(loader->add(asm_code));
			auto const& memory = compiler.get_memory();
			return std::vector<WORD>{memory.begin() + offset, memory.begin() + compiler.offset()};
		};
		code = std::make_unique<Hardware::InteractiveCode>(interactive_shell);
	} else {
		compiler.add_source(loader->get_id(argv[1]));
		code = std::make_unique<Hardware::Code>(compiler.get_memory());
	}

	Hardware::Computer computer(
		std::move(code),
		std::make_unique<Hardware::Stack>(),
		std::make_unique<Hardware::RAM>(),
		std::make_unique<Hardware::IO>(),
		std::make_unique<Hardware::Display>()
	);

	auto execute_step = [&] {
		if(computer.processor->complete())
			return;

		auto segment = compiler.get_mapping(computer.processor->get_IP());
		auto code = loader->load(segment.source_id);
		std::cerr << "ASM code(source_id = " << segment.source_id << "): " << code.substr(segment.from, segment.to - segment.from) << "\n";

		computer.execute_step([&](WORD index) { return compiler.get_mapping(index); });
	};

//	while(!computer.processor->complete())
//		execute_step();
	computer.execute(10000);
}
