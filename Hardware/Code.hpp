#pragma once
#include "Memory.hpp"

namespace Hardware {
	class Code final : public IMemory {
		std::array<WORD, CODE_SIZE> memory;
	public:
		Code(std::array<WORD, CODE_SIZE> code);
		
		WORD read(WORD address) const override;
		void write(WORD address, WORD value) override;
	};
}