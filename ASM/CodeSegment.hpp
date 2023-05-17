#pragma once

namespace ASM {
	struct CodeSegment {
		size_t from, to; //indexes in code, [from, to)

		bool operator== (CodeSegment o) const { return from == o.from && to == o.to; }
		CodeSegment& add_offset(size_t n) { from += n; to += n; return *this; };
	};
}