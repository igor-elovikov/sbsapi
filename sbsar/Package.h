#pragma once

#include "common.h"

#include "Graph.h"


namespace sbsar {

class Context;

class Package {

	friend class Context;

	std::string url;
	Context* ctx = nullptr;

public:

	sbs::GraphInstances instances;

	Package() = default;
	~Package()
	{
		graphs.clear();
		instances.clear();

		spdlog::debug("Unload package {}", url);
	}
	explicit Package(const std::string& path, Context* context, bool instantiate)
	{
		ctx = context;
		load_from_file(path, instantiate);
	}

	std::unique_ptr<sbs::PackageDesc> descriptor;
	std::vector<std::shared_ptr<sbs::GraphDesc>> graph_descriptors;
	std::unordered_map<std::string, Graph> graphs;
	std::vector<Graph> custom_instances;

	auto init_graph_descriptors() -> void;
	auto load_from_file(const std::string& path, bool instantiate = true) -> void;
	auto& graph(const std::string& id) { return graphs[id]; }
};

}



