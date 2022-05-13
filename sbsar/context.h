#pragma once

#include "common.h"
#include "package.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace sbsar {

class Context {

	auto init_logging() -> void
	{
		logger = spdlog::stdout_color_mt("sbsar");
	}

public:

	std::shared_ptr<spdlog::logger> logger;

	std::unique_ptr<sbs::Renderer> renderer;
	std::unordered_map<std::string, Package> packages;

	explicit Context(bool enable_renderer = true)
	{
		init_logging();
		if (enable_renderer) {
			renderer = std::make_unique<sbs::Renderer>();
		}
	}

	explicit Context(size_t memory_budget_mbytes)
	{
		init_logging();
		auto options = sbs::RenderOptions();
		options.mMemoryBudget = memory_budget_mbytes * 1024 * 1024;
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	explicit Context(const sbs::RenderOptions& options)
	{
		init_logging();
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	~Context()
	{
		logger->info("Shutdown SBS context");
		logger->flush();
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

