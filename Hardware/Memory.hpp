#pragma once
#include <span>
#include <cstdint>
#include <algorithm>

namespace Hardware {
	using WORD = uint32_t;
	using DWORD = uint64_t; //For overflow's checking, not used
	
	constexpr WORD CODE_SIZE = 8 * 1024 * 1024, STACK_SIZE = 8 * 1024 * 1024, RAM_SIZE = 8 * 1024 * 1024,
			IO_SIZE = 128, DISPLAY_SIZE = 1920 * 1080,
			MEMORY_SIZE = CODE_SIZE + STACK_SIZE + RAM_SIZE + IO_SIZE + DISPLAY_SIZE;
	
	constexpr WORD CODE_OFFSET = 0, STACK_OFFSET = CODE_OFFSET + CODE_SIZE, RAM_OFFSET = STACK_OFFSET + STACK_SIZE,
			IO_OFFSET = RAM_OFFSET + RAM_SIZE, DISPLAY_OFFSET = IO_OFFSET + IO_SIZE;
	
	class IMemory {
	public:
		virtual WORD read(WORD address) const = 0;
		virtual void write(WORD address, WORD value) = 0;
	};
	
	template<WORD size>
	class SimpleMemory : public IMemory {
	protected:
		std::array<WORD, size> memory;
	public:
		//zero initialized memory
		SimpleMemory() { std::ranges::fill(memory, 0); };
		
		virtual WORD read(WORD address) const override { return memory.at(address); };
		virtual void write(WORD address, WORD value) override { memory.at(address) = value; };
	};
}