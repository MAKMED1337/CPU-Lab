#pragma once
#include "../Hardware/Processor.hpp"
#include "InstructionList.hpp"
#include "ParseStream.hpp"
#include "Instructions.hpp"
#include "CodeSegment.hpp"
#include "Loader.hpp"
#include <string_view>
#include <string>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <vector>
#include <map>
#include <set>

namespace ASM {
	class Compiler final {
		ParseStream m_code;
		std::unordered_map<std::string, WORD, string_hash, std::equal_to<>> m_labels;
		std::array<WORD, Hardware::CODE_SIZE> m_memory;
		std::map<WORD, CodeSegment> m_mapping; //first instruction -> code(segment)

		std::vector<size_t> m_to_compile;
		std::set<size_t> m_imported;
		WORD m_offset = 0;

		std::shared_ptr<Loader> m_loader;

		bool skip_ws();
		/* comment start with # and ends after new line, example:
		   SET 1 #comment SET 42 <- still comment
		   SET 2
		*/
		bool skip_comment();
		bool skip();
		
		ParseStream read_token();
		std::string read_string(); //only used for import

		// types: char('a', '\\', '\'', '\n', '123'), int(base 2(0b11), 8(0o33), 16(0xF1f), 10(1234)),
		// only base 10 could be negative(contain `-` sign)
		WORD parse_argument(ParseStream token) const;
		
		static int digit_from_hex(char c);
		static WORD parse_int(std::string_view token, unsigned base = 10);
		
		static char escape_sequence(char c);

		void preprocess(size_t source_id); //changes m_offset
		void paste_source(size_t source_id);
	public:
		Compiler(std::shared_ptr<Loader> loader);

		//TODO: proper exception representation with lines/code samples
		void add_source(size_t source_id, bool execute = true);

		std::array<WORD, Hardware::CODE_SIZE> const& get_memory() const;
		CodeSegment get_mapping(WORD index) const;
	};
}