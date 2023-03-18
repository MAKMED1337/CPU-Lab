#include "Instructions.hpp"

namespace ASM {
	void init() {
#define INSTR(name, args...) InstructionList name { args }; translation[#name] = name;
		INSTR(SAVE_B, { SWAP, SWAP_C })
		INSTR(LOAD_B, { SWAP_C, SWAP })
		INSTR(SWAP_BC, { SWAP, SWAP_C, SWAP })
		
		INSTR(STOP, { SET, Hardware::Processor::STOP, WRITE_IP })
		
		INSTR(PRINT, { SWAP, SWAP_C, SET, Hardware::IO_OFFSET, WRITE, LOAD_B })
		
		INSTR(READ_RAM, { ADD, Hardware::RAM_OFFSET, READ }) //0 indexed ram
		INSTR(WRITE_RAM, { ADD, Hardware::RAM_OFFSET, WRITE }) //0 indexed ram
		
		INSTR(READ_STACK, { READ_SP, READ })
		INSTR(WRITE_STACK, { READ_SP, WRITE })
		
		INSTR(INCREASE_STACK, { SAVE_B, SET, WORD{1}, SWAP, READ_SP, ADD, WRITE_SP, LOAD_B })
		INSTR(POP_STACK, { SAVE_B, SET, WORD{1}, SWAP, READ_SP, SUB, WRITE_SP, LOAD_B })
		
		InstructionList call_without_ip {{ ADD, SWAP, INCREASE_STACK, WRITE_STACK, SET, Arg{0}, WRITE_IP }, 1};
		INSTR(CALL, { SET, call_without_ip.size(), SWAP, READ_IP, call_without_ip }, 1) //side effect on B
		
		INSTR(RET, { READ_STACK, SWAP, POP_STACK, SWAP, WRITE_IP })
	}
}