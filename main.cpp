#include <iostream>
#include "ASM/Compiler.hpp"
#include "Processor.hpp"

int main() {
	ASM::init();
	
	constexpr std::string_view asm_code = R"code(
PREPARE_ARGS
SET -1
SWAP
PUSH_STACK
CALL :print_num 1
STOP

:print_num
	READ_STACK
	JUMP_NOT_ZERO :recall
	SET '0'
	SWAP
	PRINT
	POP_STACK
	RET
	
	:recall
	READ_STACK
	SWAP
	PREPARE_ARGS
	PUSH_STACK
	CALL :print_num_inner 1
	POP_STACK
	RET

:print_num_inner
	READ_STACK
	
	JUMP_NOT_ZERO :mainline
	POP_STACK
	RET

	:mainline
	READ_STACK
	SET 10
	SWAP
	READ_STACK
	DIV
	SWAP

	PREPARE_ARGS
	PUSH_STACK
	CALL :print_num_inner 1
	
	SET 10
	SWAP
	READ_STACK
	MOD
	SWAP

	SET '0'
	ADD
	SWAP
	PRINT

	POP_STACK
	RET
)code";
	ASM::Compiler compiler(asm_code);
	auto machine_code = compiler.compile();
	
	Hardware::Processor processor(std::move(machine_code));
	
	for (int i = 0; i < 10000 && !processor.complete(); ++i) {
		processor.execute_step();
		
		auto io = processor.get_io();
		if(io[0] != 0) {
			std::cout << char(io[0]);
			io[0] = 0;
		}
	}
}
