#pragma once
#include <string_view>
#include <exception>
#include <string>

class TokenException final : public std::exception {
	std::string msg;
public:
	TokenException(const std::string& expected, const std::string& got);
	virtual const char* what() const noexcept override;
};

class ParseStream final : public std::string_view {
public:
	ParseStream(std::string_view s);
	
	//consumes if true
	bool test(std::string_view s);
	
	void expect(std::string_view s);
};
