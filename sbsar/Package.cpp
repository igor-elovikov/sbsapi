//
// Created by elovikov on 30/03/2022.
//

#include "Package.h"

namespace sbsar {

auto Package::load_from_file(const std::string& path, bool instantiate) -> void
{
	url = path;
	auto file_data = std::vector<unsigned char>{};
	load_file(file_data, path, true);

	descriptor = std::make_unique<sbs::PackageDesc>(file_data.data(), file_data.size());
	spdlog::debug("Loaded package from {}", path);

	const auto& pkg_graphs = descriptor->getGraphs();

	for (const auto& pkg_graph : descriptor->getGraphs()) {
		spdlog::debug("Loading graph: {}", pkg_graph.mPackageUrl);

		auto graph_url = std::string(pkg_graph.mPackageUrl);

		graph_url = graph_url.substr(6);

		auto& graph = graphs_container.emplace_back(pkg_graph);

		graph.ctx = ctx;
		graph.url = graph_url;

		graph.load_parameters();
		graph.load_outputs();
		graph.load_inputs();

		if (instantiate) {
			graph.instance = std::make_unique<sbs::GraphInstance>(pkg_graph);
			graph.link_instance();
		}
	}

	for (auto& graph : graphs_container) {
		graphs_map.try_emplace(graph.url, graph);
	}

}

}

