#include "ASM_Compiler.hpp"
#include <algorithm>
#include <cassert>
#include <limits>

ASM_Compiler::ASM_Compiler(std::string_view code) : m_code(code) {}

void ASM_Compiler::skip(size_t cnt) {
	m_code = m_code.substr(cnt);
}

void ASM_Compiler::skip_ws() {
	while(!m_code.empty() && std::isspace(m_code[0]))
		skip(1);
}

void ASM_Compiler::expect(std::string_view s) {
	assert(m_code.starts_with(s)); //FIXME: throw error
	skip(s.size());
}

bool ASM_Compiler::test(std::string_view s) {
	if(m_code.starts_with(s)) {
		skip(s.size());
		return true;
	}
	return false;
}

WORD ASM_Compiler::read_const() {
	skip_ws();
	
	if(test("'")) {
		char c;
		if(test("\\")) {
			c = escape_sequence(m_code[0]);
			skip(1);
		} else
			c = m_code[0];
		expect("'");
		return c;
	}
	
	if(test("0b")) return read_int(2);
	if(test("0o")) return read_int(8);
	if(test("0x")) return read_int(16);
	
	bool negative = test("-");
	auto res = read_int();
	
	if(negative)
		res = ~res + 1;
	return res;
}

WORD ASM_Compiler::read_int(unsigned int base) {
	digit_from_hex(m_code[0]); //FIXME: Do proper checking
	
	WORD res = 0;
	while(!m_code.empty()) {
		int d = digit_from_hex(m_code[0]);
		if(d == -1)
			break;
		
		assert(d < base); //FIXME: throw error
		
		auto temp = DWORD{base} * res + d;
		if (temp > std::numeric_limits<WORD>::max())
			assert(false); //FIXME: throw error
		res = temp;
		skip(1);
	}
	return res;
}

char ASM_Compiler::escape_sequence(char c) {
	// https://en.cppreference.com/w/cpp/language/escape
	switch (c) {
		case '\'':
			return '\'';
		case '"':
			return '\"';
		case '?':
			return '\?';
		case '\\':
			return '\\';
		case 'a':
			return '\a';
		case 'b':
			return '\b';
		case 'f':
			return '\f';
		case 'n':
			return '\n';
		case 'r':
			return '\r';
		case 't':
			return '\t';
		case 'v':
			return '\v';
		default:
			assert(false); //FIXME: throw error
	}
}

int ASM_Compiler::digit_from_hex(char c) {
	if('0' <= c && c <= '9')
		return c - '0';
	if('a' <= c && c <= 'z')
		return c - 'a' + 10;
	if('A' <= c && c <= 'Z')
		return c - 'A' + 10;
	return -1; //FIXME: maybe throw exception
}

memory_t ASM_Compiler::compile() {
	memory_t memory;
	WORD addr = 0;
	
	while(!m_code.empty()) {
		skip_ws();
		bool found = false;
		for(int i = 0; i < keywords.size(); ++i) {
			if(!m_code.starts_with(keywords[i]))
				continue;
			
			if(m_code.size() > keywords[i].size() && !std::isspace(m_code[keywords[i].size()]))
				continue;
			expect(keywords[i]);
			found = true;
			
			memory[addr++] = i;
			if(i == SET || i == JUMP_ZERO)
				memory[addr++] = read_const();
		}
		
		if(!found)
			assert(m_code.empty());
	}
	
	return memory;
}