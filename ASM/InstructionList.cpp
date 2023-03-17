#include "InstructionList.hpp"
#include <utility>
#include <cassert>

namespace ASM {
	template<class> inline constexpr bool always_false_v = false;
	
	WORD InstructionList::push_instructions(std::vector<WORD>&& args, std::span<WORD> memory) const {
		assert(args.size() == args_count); //FIXME: throw exception
		
		WORD ptr = 0;
		for(auto instruction : m_instructions) {
			std::visit([&](auto i) {
				using T = std::decay_t<decltype(i)>;
				if constexpr (std::is_same_v<T, Instruction>)
					memory[ptr++] = std::to_underlying(i);
				else if constexpr (std::is_same_v<T, WORD>)
					memory[ptr++] = i;
				else if constexpr (std::is_same_v<T, Arg>)
					memory[ptr++] = args[i.ind];
				else
					static_assert(always_false_v<T>, "Unknown type");
			}, instruction);
		}
		
		return ptr;
	}
}