#pragma once

#include "common.h"

#include "Graph.h"


namespace sbsar {

class Context;

class Package {

	friend class Context;

	std::string url;
	Context* ctx = nullptr;

	std::unordered_map<std::string, Graph&> graphs_map;
	std::vector<Graph> graphs_container;
	std::vector<Graph> custom_instances;

public:

	sbs::GraphInstances instances;

	Package() = default;
	~Package()
	{
		graphs_map.clear();
		graphs_container.clear();
		instances.clear();

		spdlog::debug("Unload package {}", url);
	}
	explicit Package(const std::string& path, Context* context, bool instantiate)
	{
		ctx = context;
		load_from_file(path, instantiate);
	}

	std::unique_ptr<sbs::PackageDesc> descriptor;

	auto load_from_file(const std::string& path, bool instantiate = true) -> void;
	[[nodiscard]] auto& graph(const std::string& id) const { return graphs_map.at(id); }
	[[nodiscard]] const auto& graphs() const { return graphs_container; }
};

}



