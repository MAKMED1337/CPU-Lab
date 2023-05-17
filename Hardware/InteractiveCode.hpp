#pragma once
#include "Memory.hpp"

namespace Hardware {
	class InteractiveCode final : public IMemory {
		mutable std::array<WORD, CODE_SIZE> memory{};
		mutable WORD m_initialized_until = 0;
		std::function<std::vector<WORD>()> m_generator;
	public:
		InteractiveCode(std::function<std::vector<WORD>()> code_generator);

		WORD read(WORD address) const override;
		void write(WORD address, WORD value) override;
	};
}