#pragma once
#include "Memory.hpp"
#include <array>

namespace Hardware {
	class RAM final : public SimpleMemory<RAM_SIZE> {};
}