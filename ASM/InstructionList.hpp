#pragma once
#include "../Hardware/Processor.hpp"
#include <variant>
#include <vector>
#include <span>

namespace ASM {
	using Hardware::WORD;
	using Instruction = Hardware::Processor::Instruction;
	
	struct Arg {
		WORD ind;
	};
	
	class InstructionList final {
		using instruction_type = std::variant<Instruction, WORD, Arg>;
		std::vector<instruction_type> m_instructions;
		size_t args_count;
	public:
		size_t get_args_count() const;
		WORD size() const;
		
		using constructor_type = std::variant<Instruction, WORD, Arg, InstructionList>;
		InstructionList(std::vector<constructor_type> const& instructions = {}, size_t arguments_count = 0);
		
		using argument_type = std::variant<WORD, Arg>;
		InstructionList remap(std::vector<argument_type> const& args = {});
		
		InstructionList& extend(InstructionList const& other);
		
		//returns count of instructions
		WORD push_instructions(std::vector<WORD>&& args, std::span<WORD> memory) const;
	};
}