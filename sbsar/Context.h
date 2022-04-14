#pragma once

#include "common.h"
#include "Package.h"

namespace sbsar {

class Context {

public:

	sbs::Renderer renderer;
	std::unordered_map<std::string, Package> packages;

	Context()
	{
		spdlog::info("Start SBS context");
	}

	~Context()
	{
		spdlog::info("Shutdown SBS context");
	}

	auto load_package(const std::string& filename, bool instantiate = true) -> Package&
	{
		if (packages.contains(filename)) return packages[filename];
		auto [item, result] = packages.try_emplace(filename, filename, this, instantiate);
		auto& new_package = item->second;
		return new_package;
	}
};

}

