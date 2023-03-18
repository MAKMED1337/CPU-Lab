#pragma once
#include <string_view>
#include <stdexcept>
#include <string>

class TokenException final : public std::logic_error {
public:
	TokenException(const std::string& expected, const std::string& got);
};

class ParseStream final : public std::string_view {
public:
	ParseStream(std::string_view s);
	
	//consumes if true
	bool test(std::string_view s);
	
	void expect(std::string_view s);
};
