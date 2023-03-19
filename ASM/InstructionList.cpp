#include "InstructionList.hpp"
#include <utility>
#include <cassert>
#include <stdexcept>
#include <fmt/format.h>

namespace ASM {
	template<class> inline constexpr bool always_false_v = false;
	
	WORD InstructionList::push_instructions(std::vector<WORD>&& args, std::span<WORD> memory) const {
		if (args.size() != args_count)
			throw std::invalid_argument(fmt::format("Expected exactly {} argument(s), but got {} argument(s)", args_count, args.size()));
		
		WORD ptr = 0;
		for(auto instruction : m_instructions) {
			std::visit([&](auto instr) {
				using T = std::decay_t<decltype(instr)>;
				if constexpr (std::is_same_v<T, Instruction>)
					memory[ptr++] = std::to_underlying(instr);
				else if constexpr (std::is_same_v<T, WORD>)
					memory[ptr++] = instr;
				else if constexpr (std::is_same_v<T, Arg>)
					memory[ptr++] = args[instr.ind];
				else
					static_assert(always_false_v<T>, "Unknown type");
			}, instruction);
		}
		
		return ptr;
	}
	
	InstructionList& InstructionList::extend(const InstructionList& other) {
		m_instructions.insert(end(m_instructions), other.m_instructions.begin(), other.m_instructions.end());
		return *this;
	}
	
	size_t InstructionList::get_args_count() const {
		return args_count;
	}
	
	InstructionList::InstructionList(std::vector<constructor_type> const& instructions, size_t arguments_count)
		: args_count(arguments_count) {
		for (auto const& instruction : instructions) {
			std::visit([&](auto const& i) {
				using T = std::decay_t<decltype(i)>;
				if constexpr (std::is_same_v<T, InstructionList>)
					extend(i);
				else
					m_instructions.push_back(i);
			}, instruction);
		}
	}
	
	WORD InstructionList::size() const {
		return m_instructions.size();
	}
	
	InstructionList InstructionList::remap(const std::vector<argument_type>& args) {
		assert(args_count == args.size());
		
		std::vector<instruction_type> res(m_instructions.size());
		for(size_t i = 0; i < res.size(); ++i) {
			std::visit([&](auto instr) {
				using T = std::decay_t<decltype(instr)>;
				if constexpr (std::is_same_v<T, Arg>) {
					auto const& arg = args[instr.ind];
					if (std::holds_alternative<WORD>(arg))
						res[i] = std::get<WORD>(arg);
					else
						res[i] = std::get<Arg>(arg);
				} else
					res[i] = instr;
			}, m_instructions[i]);
		}
	}
}