#pragma once
#include "Memory.hpp"

namespace Hardware {
	class IO final : public IMemory {
		WORD read(WORD address) const override;
		void write(WORD address, WORD value) override;
	};
}