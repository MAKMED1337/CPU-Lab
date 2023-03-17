#include <iostream>
#include "ASM/Compiler.hpp"
#include "Processor.hpp"

int main() {
	constexpr std::string_view asm_code = R"code(
SET 5
SWAP
SET 10
ADD
STOP
)code";
	ASM::Compiler compiler(asm_code);
	auto machine_code = compiler.compile();
	
	Hardware::Processor processor(std::move(machine_code));
	while (!processor.complete()) {
		processor.execute_step();
		processor.dump(std::cerr);
	}
}
