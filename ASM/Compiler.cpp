#include "Compiler.hpp"
#include <algorithm>
#include <cassert>
#include <limits>

namespace ASM {
	Compiler::Compiler(std::string_view code) : m_code(code) {}
	
	void Compiler::skip_ws() {
		while (!m_code.empty() && std::isspace(m_code[0]))
			m_code.remove_prefix(1);
	}
	
	WORD Compiler::read_argument() {
		ParseStream token = read_token();
		assert(!token.empty()); //FIXME: throw error
		
		if (token.test("'")) {
			char c = token.test("\\") ? escape_sequence(token[0]) : token[0];
			token.remove_prefix(1);
			token.expect("'");
			return c;
		}
		
		if (token.test("0b")) return parse_int(token, 2);
		if (token.test("0o")) return parse_int(token, 8);
		if (token.test("0x")) return parse_int(token, 16);
		
		bool negative = token.test("-");
		auto res = parse_int(token);
		
		if (negative)
			res = ~res + 1;
		return res;
	}
	
	WORD Compiler::parse_int(std::string_view token, unsigned int base) {
		assert(token.size() > 0); //FIXME: throw error
		
		WORD res = 0;
		for(auto i : token) {
			int d = digit_from_hex(i);
			assert(d != -1); //FIXME: throw error
			assert(d < base); //FIXME: throw error
			
			auto temp = Hardware::DWORD{ base } * res + d;
			if (temp > std::numeric_limits<WORD>::max()) //overflow compile time check
				assert(false); //FIXME: throw error
			res = temp;
		}
		return res;
	}
	
	char Compiler::escape_sequence(char c) {
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
	
	int Compiler::digit_from_hex(char c) {
		if ('0' <= c && c <= '9')
			return c - '0';
		if ('a' <= c && c <= 'z')
			return c - 'a' + 10;
		if ('A' <= c && c <= 'Z')
			return c - 'A' + 10;
		return -1; //FIXME: maybe throw exception
	}
	
	memory_t Compiler::compile() {
		memory_t memory;
		memory.fill(0);
		
		std::span<WORD> space = memory;
		
		while (true) {
			auto token = read_token();
			if (token.empty())
				break;
			
			auto it = keywords.find(token);
			assert(it != keywords.end()); //FIXME: throw exception
			
			auto const& instruction = it->second;
			std::vector<WORD> args(instruction.args_count);
			for(auto& i : args)
				i = read_argument();
			
			auto count = instruction.push_instructions(std::move(args), space);
			space = space.subspan(count);
		}
		
		return memory;
	}
	
	std::string_view Compiler::read_token() {
		skip_ws();
		WORD i = 0;
		while(i < m_code.size() && !std::isspace(m_code[i]))
			++i;
		
		auto res = m_code.substr(0, i);
		m_code.remove_prefix(i);
		return res;
	}
};