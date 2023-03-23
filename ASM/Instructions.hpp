#pragma once
#include "../Processor.hpp"
#include "InstructionList.hpp"
#include <string_view>
#include <string>
#include <unordered_map>
#include <utility>

namespace ASM {
	struct string_hash
	{
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;
		
		std::size_t operator()(const char* str) const        { return hash_type{}(str); }
		std::size_t operator()(std::string_view str) const   { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};
	
	using enum Instruction;
	inline std::unordered_map<std::string, InstructionList, string_hash, std::equal_to<>> translation = {
		{"READ", InstructionList{{ READ }}},
		{"WRITE", InstructionList{{ WRITE }}},
		{"SWAP", InstructionList{{ SWAP }}},
		{"SWAP_C", InstructionList{{ SWAP_C }}},
		{"SET", InstructionList{{ SET, Arg{0} }, 1}},
		{"JUMP_ZERO", InstructionList{{ JUMP_ZERO, Arg{0} }, 1}},
		{"READ_IP", InstructionList{{ READ_IP }}},
		{"WRITE_IP", InstructionList{{ WRITE_IP }}},
		{"READ_SP", InstructionList{{ READ_SP }}},
		{"WRITE_SP", InstructionList{{ WRITE_SP }}},
		{"ADD", InstructionList{{ ADD }}},
		{"SUB", InstructionList{{ SUB }}},
		{"MULT", InstructionList{{ MULT }}},
		{"DIV", InstructionList{{ DIV }}},
		{"MOD", InstructionList{{ MOD }}},
		{"SHIFT_L", InstructionList{{ SHIFT_L }}},
		{"SHIFT_R", InstructionList{{ SHIFT_R }}},
		{"AND", InstructionList{{ AND }}},
		{"OR", InstructionList{{ OR }}},
		{"XOR", InstructionList{{ XOR }}},
		{"NOT", InstructionList{{ NOT }}},
		{"LNOT", InstructionList{{ LNOT }}},
		{"DUMP", InstructionList{{ DUMP }}},
		{"HELLO", InstructionList{{ HELLO }}},
	};
	
	//init custom commands
	void init();
}
