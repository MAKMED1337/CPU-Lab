#include "Loader.hpp"
#include <fstream>
#include <sstream>
#include <utility>

namespace ASM {
	size_t Loader::get_id(std::string_view name) {
		auto it = m_ids.find(name);
		if (it != m_ids.end())
			return it->second;

		std::ifstream file(m_root / name);
		std::ostringstream buffer;
		buffer << file.rdbuf();
		data.push_back(buffer.str());

		return m_ids[std::string{name}] = data.size() - 1;
	}

	std::string_view Loader::load(size_t id) {
		return data[id];
	}

	size_t Loader::add(std::string const& content) {
		data.emplace_back(content);
		return data.size() - 1;
	}

	Loader::Loader(fs::path root) : m_root(std::move(root)) {}
}