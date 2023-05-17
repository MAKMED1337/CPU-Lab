#pragma once

namespace ASM {
	struct CodeSegment {
		size_t source_id;
		size_t from, to; //indexes in code, [from, to)

		bool operator== (CodeSegment o) const { return source_id == o.source_id && from == o.from && to == o.to; }
		CodeSegment& add_offset(size_t n) { from += n; to += n; return *this; };
	};
}