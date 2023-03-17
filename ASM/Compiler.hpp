#pragma once
#include "../Processor.hpp"
#include "InstructionList.hpp"
#include "ParseStream.hpp"
#include <string_view>
#include <string>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <vector>
#include <variant>

using Hardware::WORD, Hardware::Instruction, Hardware::memory_t;
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
	const std::unordered_map<std::string, InstructionList, string_hash, std::equal_to<>> keywords = {
		{"READ", InstructionList{{ READ }}},
		{"WRITE", InstructionList{{ WRITE }}},
		{"SWAP", InstructionList{{ SWAP }}},
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
		{"SHIFT_L", InstructionList{{ SHIFT_L }}},
		{"SHIFT_R", InstructionList{{ SHIFT_R }}},
		{"AND", InstructionList{{ AND }}},
		{"OR", InstructionList{{ OR }}},
		{"XOR", InstructionList{{ XOR }}},
		
		
		
		{"STOP", InstructionList{{ SET, Hardware::Processor::STOP, WRITE_IP }}},
	};
	
	class Compiler final {
		ParseStream m_code;
		
		void skip_ws();
		std::string_view read_token();
		
		// types: char('a', '\\', '\'', '\n', '123'), int(base 2(0b11), 8(0o33), 16(0xF1f), 10(1234)),
		// only base 10 could be negative(contain `-` sign)
		WORD read_argument();
		
		static WORD parse_int(std::string_view token, unsigned base = 10);
		
		static char escape_sequence(char c);
		
		static int digit_from_hex(char c);
	
	public:
		Compiler(std::string_view code);
		
		memory_t compile();
	};
};