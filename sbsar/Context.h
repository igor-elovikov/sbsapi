#pragma once

#include "common.h"
#include "Package.h"

namespace sbsar {

class Context {

public:

	std::unique_ptr<sbs::Renderer> renderer;
	std::unordered_map<std::string, Package> packages;

	explicit Context(bool enable_renderer = true)
	{
		spdlog::info("Start SBS context");
		if (enable_renderer) {
			renderer = std::make_unique<sbs::Renderer>();
		}

	}

	explicit Context(size_t memory_budget_mbytes)
	{
		auto options = sbs::RenderOptions();
		options.mMemoryBudget = memory_budget_mbytes * 1024 * 1024;
		spdlog::info("Start SBS context with {}MB memory budget", memory_budget_mbytes);
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	explicit Context(const sbs::RenderOptions& options)
	{
		spdlog::info("Start SBS context");
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	~Context()
	{
		spdlog::info("Shutdown SBS context");
	}

	auto set_renderer_options(const sbs::RenderOptions& options) const -> void
	{
		if (!renderer) return;
		renderer->setOptions(options);
		renderer->run();
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

