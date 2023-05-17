#include "InteractiveCode.hpp"
#include <stdexcept>
#include <utility>
#include <fmt/format.h>

namespace Hardware {
	WORD InteractiveCode::read(WORD address) const {
		if(address > m_initialized_until)
			throw std::logic_error(fmt::format("Invalid address {}, initialized until {}", address, m_initialized_until));

		if(address == m_initialized_until) {
			auto code = m_generator();
			for(auto i : code)
				memory[m_initialized_until++] = i;
		}

		if(address < m_initialized_until)
			return memory.at(address);
	}

	void InteractiveCode::write(Hardware::WORD address, Hardware::WORD value) {
		throw std::invalid_argument("Code section is unwritable"); //TODO: rethink if this necessary
	}

	InteractiveCode::InteractiveCode(std::function<std::vector<WORD>()> code_generator) : m_generator(std::move(code_generator)) {}
}