#include <iostream>
#include "ASM/Compiler.hpp"
#include "Processor.hpp"

int main() {
	constexpr std::string_view asm_code = R"code(
SET 5
SWAP
SET 10
ADD
SET 0xFFFFFFFF
WRITE_IP
)code";
	ASM::Compiler compiler(asm_code);
	auto machine_code = compiler.compile();
	
	Hardware::Processor processor(std::move(machine_code));
	processor.execute();
}
