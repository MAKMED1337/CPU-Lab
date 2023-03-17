#pragma once
#include "../Processor.hpp"
#include <variant>
#include <vector>
#include <span>

using Hardware::WORD, Hardware::Instruction;
namespace ASM {
	struct Arg {
		WORD ind;
	};
	
	class InstructionList final {
		using type = std::variant<Instruction, WORD, Arg>;
		std::vector<type> m_instructions;
	public:
		const unsigned args_count;
		
		InstructionList(std::vector<type> instructions = {}, unsigned arguments_count = 0) :
			m_instructions(std::move(instructions)), args_count(arguments_count) {}
		
		//returns count of instructions
		WORD push_instructions(std::vector<WORD>&& args, std::span<WORD> memory) const;
	};
}