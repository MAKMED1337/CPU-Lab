#pragma once
#include <string_view>
#include <cassert>
#include "Processor.hpp"

class ASM_Compiler final {
	std::string_view m_code;
	
	void skip(size_t cnt);
	bool test(std::string_view s);
	void expect(std::string_view s);
	void skip_ws();
	
	//types: char('a', '\\', '\'', '\n', '123'), int(base 2(0b11), 8(0o33), 16(0xF1f), 10(1234)), only base 10 could be negative
	WORD read_const();
	WORD read_int(unsigned base = 10);
	
	static char escape_sequence(char c);
	static int digit_from_hex(char c);
public:
	ASM_Compiler(std::string_view code);
	memory_t compile();
};
