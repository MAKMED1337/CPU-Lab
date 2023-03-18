#include "ParseStream.hpp"
#include <fmt/format.h>

TokenException::TokenException(const std::string& expected, const std::string& got) :
	logic_error(fmt::format("Expected: `{}`, but got `{}`", expected, got))
{}



ParseStream::ParseStream(std::string_view s) : std::string_view(s) {}

bool ParseStream::test(std::string_view s) {
	if(starts_with(s)) {
		remove_prefix(s.size());
		return true;
	}
	return false;
}

void ParseStream::expect(std::string_view s) {
	if(!test(s))
		throw TokenException(std::string{s}, std::string{substr(0, s.size())});
}