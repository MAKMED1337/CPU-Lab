#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include "Instructions.hpp"
#include <filesystem>

namespace fs = std::filesystem;
namespace ASM {
	class Loader final { //file loader
	public:
		Loader(fs::path root);

		size_t get_id(std::string_view name); //loads from root(exe file) folder
		size_t add(std::string const& content);
		std::string_view load(size_t id); //TODO: return string or string_view ?
	private:
		std::unordered_map<std::string, size_t, string_hash, std::equal_to<>> m_ids;
		std::vector<std::string> data;
		size_t max_id = 0;
		fs::path m_root;
	};
}