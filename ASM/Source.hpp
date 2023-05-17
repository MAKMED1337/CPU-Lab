#pragma once
#include <string_view>

namespace ASM {
	struct Source {
		size_t id;
		std::string_view code;
	};
}