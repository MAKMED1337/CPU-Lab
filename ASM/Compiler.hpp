#pragma once
#include "../Processor.hpp"
#include "InstructionList.hpp"
#include "ParseStream.hpp"
#include "Instructions.hpp"
#include <string_view>
#include <string>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <vector>

using Hardware::WORD, Hardware::Instruction, Hardware::memory_t;
namespace ASM {
	class Compiler final {
		ParseStream m_code;
		std::unordered_map<std::string, WORD, string_hash, std::equal_to<>> labels;
		
		bool skip_ws();
		/* comment start with # and ends after new line, example:
		   SET 1 #comment SET 42 <- still comment
		   SET 2
		*/
		bool skip_comment();
		bool skip();
		
		ParseStream read_token();
		
		// types: char('a', '\\', '\'', '\n', '123'), int(base 2(0b11), 8(0o33), 16(0xF1f), 10(1234)),
		// only base 10 could be negative(contain `-` sign)
		WORD parse_argument(ParseStream token) const;
		
		static int digit_from_hex(char c);
		static WORD parse_int(std::string_view token, unsigned base = 10);
		
		static char escape_sequence(char c);
		
		// changing m_code state
		// it parsing only labels rn, but in future can be used to parse more things
		void preparse();
	public:
		Compiler(std::string_view code);
		
		//TODO: proper exception representation with lines/code samples
		memory_t compile();
	};
}