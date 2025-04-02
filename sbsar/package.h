#pragma once

#include "../utils/common.h"
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
	Package(const Package&) = delete;
	Package(Package&&) = default;
	~Package();

	std::unique_ptr<sbs::PackageDesc> descriptor;

	[[nodiscard]] auto graph(const std::string& id) -> Graph* {
		if (auto graph = graphs_map.find(id); graph != graphs_map.end()) {
			return &graph->second;
		}
		else {
			return nullptr;
		}
	}

	[[nodiscard]] auto& graphs() { return graphs_container; }
};

}



