#include <iostream>
#include "ASM/Compiler.hpp"
#include "Processor.hpp"

int main() {
	constexpr std::string_view asm_code = R"code(
SET 5
SWAP
SET 3
ADD
SWAP
SET '0'
ADD
PRINT
STOP
)code";
	ASM::Compiler compiler(asm_code);
	auto machine_code = compiler.compile();
	
	Hardware::Processor processor(std::move(machine_code));
	while (!processor.complete()) {
		processor.execute_step();
		auto io = processor.get_io();
		if(io[0] != 0) {
			std::cout << char(io[0]);
			io[0] = 0;
		}
	}
}
