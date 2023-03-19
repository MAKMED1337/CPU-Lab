#include "Instructions.hpp"

namespace ASM {
	void init() {
#define INTERNAL(name, args...) InstructionList name { args };
#define INSTR(name, args...) INTERNAL(name, args) translation[#name] = name;
		INSTR(SAVE_B, { SWAP, SWAP_C }) //A, C
		INSTR(LOAD_B, { SWAP_C, SWAP }) //A, C
		INSTR(SWAP_BC, { SWAP, SWAP_C, SWAP })  //A <-> B
		
		INSTR(JUMP, { SET, Arg{0}, WRITE_IP }, 1) //A
		INSTR(JUMP_NOT_ZERO, { LNOT, JUMP_ZERO, Arg{0} }, 1) //A
		
		INSTR(STOP, { SET, Hardware::Processor::STOP, WRITE_IP })
		INSTR(COPY_B, { SET, WORD{0}, ADD }) //A <- B
		INSTR(COPY_A, { SWAP,COPY_B }) //B <- A
		
		INSTR(PRINT, { SET, Hardware::IO_OFFSET, WRITE }) //A
		
		INSTR(READ_RAM, { ADD, Hardware::RAM_OFFSET, READ }) //A, 0 indexed ram
		INSTR(WRITE_RAM, { ADD, Hardware::RAM_OFFSET, WRITE }) //A, 0 indexed ram
		
		INSTR(READ_STACK, { READ_SP, READ }) //A
		INSTR(WRITE_STACK, { READ_SP, WRITE }) //A
		
		INSTR(EXTEND_STACK, { SAVE_B, SET, WORD{1}, SWAP, READ_SP, ADD, WRITE_SP, LOAD_B }) //A, C
		INSTR(PUSH_STACK, { EXTEND_STACK, WRITE_STACK }) //A, C
		INSTR(POP_STACK, { SAVE_B, SET, WORD{1}, SWAP, READ_SP, SUB, WRITE_SP, LOAD_B }) //A, C
		
		InstructionList call_without_ip {{ ADD, SWAP, INCREASE_STACK, WRITE_STACK, SET, Arg{0}, WRITE_IP }, 1};
		INSTR(CALL, { SET, call_without_ip.size(), SWAP, READ_IP, call_without_ip }, 1) //side effect on B
		
		INSTR(RET, { READ_STACK, SWAP, POP_STACK, SWAP, WRITE_IP })
	}
}