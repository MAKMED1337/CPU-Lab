#include "Compiler.hpp"
#include <limits>
#include <stdexcept>
#include <fmt/format.h>

namespace ASM {
	Compiler::Compiler(std::string_view code) : m_code(code) {}
	
	bool Compiler::skip_ws() {
		if (!std::isspace(m_code[0]))
			return false;
		
		while (!m_code.empty() && std::isspace(m_code[0]))
			m_code.remove_prefix(1);
		return true;
	}
	
	bool Compiler::skip_comment() {
		if(!m_code.test("#"))
			return false;
		
		while (!m_code.empty() && !m_code.test("\n"))
			m_code.remove_prefix(1);
		return true;
	}
	
	bool Compiler::skip() {
		bool skipped = false;
		while(skip_ws() || skip_comment())
			skipped = true;
		return skipped;
	}
	
	WORD Compiler::parse_argument(ParseStream token) const {
		if (token.empty())
			throw std::invalid_argument("Expected argument");
		
		if (token.test(":")) {
			auto it = labels.find(token);
			if (it == labels.end())
				throw std::invalid_argument(fmt::format("No such label as `:{}`", token));
			return it->second;
		}
		
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
		if (token.empty())
			throw std::invalid_argument("Expected number");
		
		WORD res = 0;
		for(auto i : token) {
			int d = digit_from_hex(i);
			
			if (d >= base)
				throw std::invalid_argument(fmt::format("Expected digit in base {}, got: '{}' = {}", std::to_string(base), i, d));
			
			auto temp = Hardware::DWORD{ base } * res + d;
			if (temp > std::numeric_limits<WORD>::max()) //overflow compile time check
				throw std::invalid_argument(fmt::format("Number {} - too large", token));
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
				throw std::invalid_argument(fmt::format("Can't escape sequence `{}`", c));
		}
	}
	
	int Compiler::digit_from_hex(char c) {
		if ('0' <= c && c <= '9')
			return c - '0';
		if ('a' <= c && c <= 'z')
			return c - 'a' + 10;
		if ('A' <= c && c <= 'Z')
			return c - 'A' + 10;
		
		throw std::invalid_argument(fmt::format("Expected digit, got: `{}`", c));
	}
	
	void Compiler::preparse() {
		WORD offset = 0;
		
		while (true) {
			auto token = read_token();
			if (token.empty())
				break;
			
			if (token.test(":")) {
				labels[std::string {token}] = offset;
				continue;
			}
			
			auto it = translation.find(token);
			if (it == translation.end())
				throw std::logic_error(fmt::format("Expected token, got: `{}`", token));
			
			auto const& instruction = it->second;
			for(size_t i = 0; i < instruction.get_args_count(); ++i)
				read_token();
			offset += instruction.size();
		}
	}
	
	memory_t Compiler::compile() {
		auto code = m_code;
		preparse();
		m_code = code;
		
		memory_t memory;
		memory.fill(0);
		std::span<WORD> as_span(memory);
		
		WORD offset = 0;
		while (true) {
			auto token = read_token();
			if (token.empty())
				break;
			
			if (token.test(":"))
				continue;
			
			auto it = translation.find(token);
			if (it == translation.end())
				throw std::logic_error(fmt::format("Expected token, got: `{}`", token));
			
			auto const& instruction = it->second;
			std::vector<WORD> args(instruction.get_args_count());
			for(auto& i : args)
				i = parse_argument(read_token());
			offset += instruction.push_instructions(std::move(args), as_span.subspan(offset));
		}
		
		return memory;
	}
	
	ParseStream Compiler::read_token() {
		skip();
		WORD i = 0;
		while(i < m_code.size() && !std::isspace(m_code[i]))
			++i;
		
		auto res = m_code.substr(0, i);
		m_code.remove_prefix(i);
		return res;
	}
}