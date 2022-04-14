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
	init_graph_descriptors();

	spdlog::debug("Loaded package from {}", path);
	auto& pkg_graphs = descriptor->getGraphs();

	for (const auto& pkg_graph : graph_descriptors) {
		spdlog::debug("Loading graph: {}", pkg_graph->mPackageUrl);

		auto last_slash = pkg_graph->mPackageUrl.rfind('/');
		auto graph_url = std::string(pkg_graph->mPackageUrl);

		if (last_slash != std::string::npos) {
			graph_url = graph_url.substr(last_slash + 1);
		}

		auto [graph_elem, result] = graphs.try_emplace(graph_url, *pkg_graph);
		auto& graph = graph_elem->second;

		graph.ctx = ctx;

		graph.descriptor = pkg_graph;
		graph.url = graph_url;

		graph.load_parameters();
		graph.load_outputs();
		graph.load_inputs();

		if (instantiate) {
			graph.instance = std::make_unique<sbs::GraphInstance>(*pkg_graph);
			graph.link_instance();
		}
	}

}
auto Package::init_graph_descriptors() -> void
{
	graph_descriptors.clear();
	for (const auto& pkg_graph : descriptor->getGraphs()) {
		graph_descriptors.push_back(std::make_shared<sbs::GraphDesc>(pkg_graph));
	}
}

}

