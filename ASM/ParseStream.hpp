#pragma once
#include <string_view>
#include <stdexcept>
#include <string>

namespace ASM {
	class TokenException final : public std::logic_error {
	public:
		TokenException(const std::string &expected, const std::string &got);
	};

	class ParseStream final {
		std::string_view m_string;
		size_t m_offset = 0;
	public:
		ParseStream() = default;

		ParseStream(std::string_view s);

		void set(std::string_view s);

		//consumes if true
		bool test(std::string_view s);

		void expect(std::string_view s);

		size_t offset() const;

		void remove_prefix(size_t n);

		bool empty() const;

		char front() const;

		char operator[](size_t index) const;

		ParseStream substr(size_t pos, size_t n) const;

		size_t size() const;

		operator std::string_view() const;
	};
}