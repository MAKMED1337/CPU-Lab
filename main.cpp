#include <iostream>
#include "ASM/Compiler.hpp"
#include "Processor.hpp"

int main() {
	ASM::init();
	
	constexpr std::string_view asm_code = R"code(
CALL :F
CALL :F
SET 'E'
PRINT
STOP

:F
SET 'F'
PRINT
RET
)code"; //expected "FFE"
	ASM::Compiler compiler(asm_code);
	auto machine_code = compiler.compile();
	
	Hardware::Processor processor(std::move(machine_code));
	while (!processor.complete()) {
		processor.execute_step();
		//processor.dump(std::cerr);
		//processor.dump_stack(std::cerr);
		
		auto io = processor.get_io();
		if(io[0] != 0) {
			std::cout << char(io[0]);
			io[0] = 0;
		}
	}
}
