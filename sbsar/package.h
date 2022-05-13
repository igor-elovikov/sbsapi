#pragma once

#include "common.h"
#include "graph.h"

namespace sbsar {

class Package {

	friend class Context;

	std::string url;
	Context* ctx = nullptr;

	std::vector<Graph> graphs_container;
	std::unordered_map<std::string, Graph&> graphs_map;

	std::vector<Graph> custom_instances;

public:

	sbs::GraphInstances instances;

	Package() = default;
	~Package();

	explicit Package(const std::string& path, Context* context, bool instantiate)
	{
		ctx = context;
		load_from_file(path, instantiate);
	}

	std::unique_ptr<sbs::PackageDesc> descriptor;

	auto load_from_file(const std::string& path, bool instantiate = true) -> void;
	[[nodiscard]] auto& graph(const std::string& id) { return graphs_map.at(id); }
	[[nodiscard]] auto& graphs() { return graphs_container; }
};

}



