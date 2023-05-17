#include "Compiler.hpp"
#include <limits>
#include <stdexcept>
#include <fmt/format.h>
#include <ranges>
#include <utility>

namespace ranges = std::ranges;
namespace ASM {
	Compiler::Compiler(std::shared_ptr<Loader> loader) : m_loader(std::move(loader)) {
		m_memory.fill(0);
	}
	
	bool Compiler::skip_ws() {
		if (!std::isspace(m_code.front()))
			return false;
		
		while (!m_code.empty() && std::isspace(m_code.front()))
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
			auto it = m_labels.find(std::string_view {token});
			if (it == m_labels.end())
				throw std::invalid_argument(fmt::format("No such label as `:{}`", token));
			return it->second;
		}
		
		if (token.test("'")) {
			char c = token.test("\\") ? escape_sequence(token.front()) : token.front();
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
	
	void Compiler::preprocess(size_t source_id) {
		m_imported.insert(source_id);
		m_to_compile.push_back(source_id);

		m_code.set(m_loader->load(source_id));

		std::vector<size_t> sources; //import them
		while (true) {
			auto token = read_token();
			if (token.empty())
				break;
			
			if (token.test(":")) {
				if(m_labels.contains(std::string_view{token}))
					throw std::invalid_argument(fmt::format("Label `{}` already declared", token));

				m_labels[std::string{token}] = m_offset;
				continue;
			}

			if (token.test("import")) {
				auto location = read_string();
				auto id = m_loader->get_id(location);
				if(!m_imported.contains(id)) {
					m_imported.insert(id);
					sources.push_back(id);
					m_offset += translation["JUMP"].size();
				}
				continue;
			}
			
			auto it = translation.find(std::string_view {token});
			if (it == translation.end())
				throw std::logic_error(fmt::format("Expected token, got: `{}`", token));
			
			auto const& instruction = it->second;
			for(size_t i = 0; i < instruction.get_args_count(); ++i)
				read_token();
			m_offset += instruction.size();
		}

		for(auto id : sources)
			preprocess(id);
	}

	void Compiler::paste_source(size_t source_id) {
		auto start = m_offset;

		m_code.set(m_loader->load(source_id));

		std::span<WORD> as_span(m_memory);
		while (true) {
			skip();
			CodeSegment segment{ source_id, m_code.offset() };

			auto token = read_token();
			if (token.empty())
				break;

			if (token.test(":"))
				continue;

			if(token.test("import")) {
				read_string();
				continue;
			}

			auto it = translation.find(std::string_view{token});
			if (it == translation.end())
				throw std::logic_error(fmt::format("Expected token, got: `{}`", token));

			auto const& instruction = it->second;
			std::vector<WORD> args(instruction.get_args_count());
			for(auto& i : args)
				i = parse_argument(read_token());

			segment.to = m_code.offset();

			WORD count = instruction.push_instructions(std::move(args), as_span.subspan(m_offset));
			m_mapping[m_offset] = segment;
			m_offset += count;
		}

		fmt::print("Loaded source({}) - [{}, {}) instructions\n", source_id, start, m_offset);
	}

	void Compiler::add_source(size_t source_id, bool execute) {
		assert(!execute || !m_imported.contains(source_id)); //import or execute(not imported before)

		if(m_imported.contains(source_id))
			return;

		auto offset = m_offset;
		preprocess(source_id);
		m_offset = offset;

		std::span<WORD> as_span(m_memory);
		for(auto id : m_to_compile) {
			bool skip_source = id != source_id || !execute;
			auto header_offset = m_offset;

			if(skip_source)
				m_offset += translation["JUMP"].size();

			paste_source(id);

			if (skip_source)
				translation["JUMP"].push_instructions({m_offset}, as_span.subspan(header_offset));
		}
		m_to_compile.clear();
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

	std::array<WORD, Hardware::CODE_SIZE> const& Compiler::get_memory() const {
		return m_memory;
	}

	CodeSegment Compiler::get_mapping(WORD index) const {
		return (--m_mapping.upper_bound(index))->second;
	}

	std::string Compiler::read_string() {
		skip();
		m_code.expect("\"");

		std::string res;
		while (!m_code.empty()) {
			bool escaped = m_code.test("\\");
			if(!escaped && m_code.test("\""))
				return res;

			res += escaped ? escape_sequence(m_code.front()) : m_code.front();
			m_code.remove_prefix(1);
		}

		throw std::invalid_argument("Unclosed string at the of the file");
	}

	WORD Compiler::offset() {
		return m_offset;
	}
}