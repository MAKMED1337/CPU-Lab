#include "ParseStream.hpp"
#include <fmt/format.h>
#include <cassert>

TokenException::TokenException(const std::string& expected, const std::string& got) :
	logic_error(fmt::format("Expected: `{}`, but got `{}`", expected, got))
{}


ParseStream::ParseStream(std::string_view s) : m_string(s) {}

bool ParseStream::test(std::string_view s) {
	if(m_string.starts_with(s)) {
		remove_prefix(s.size());
		return true;
	}
	return false;
}

void ParseStream::expect(std::string_view s) {
	if(!test(s))
		throw TokenException(std::string{s}, std::string{substr(0, s.size())});
}

void ParseStream::set(std::string_view s) {
	assert(empty());
	m_string = s;
	m_offset = 0;
}

bool ParseStream::empty() const {
	return m_string.empty();
}

void ParseStream::remove_prefix(size_t n) {
	m_string.remove_prefix(n);
	m_offset += n;
}

ParseStream::operator std::string_view() const {
	return m_string;
}

char ParseStream::front() const {
	return m_string.front();
}

char ParseStream::operator[](size_t index) const {
	return m_string[index];
}

ParseStream ParseStream::substr(size_t pos, size_t n) const {
	ParseStream res{m_string.substr(pos, n) };
	res.m_offset = m_offset + n;
	return res;
}

size_t ParseStream::size() const {
	return m_string.size();
}

size_t ParseStream::offset() const {
	return m_offset;
}
