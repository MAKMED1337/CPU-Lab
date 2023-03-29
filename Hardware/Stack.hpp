#pragma once
#include "Memory.hpp"

namespace Hardware {
	class Stack final : public SimpleMemory<STACK_SIZE> {};
}