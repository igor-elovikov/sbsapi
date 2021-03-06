//
// Created by elovikov on 30/03/2022.
//

#include "package.h"
#include "context.h"

namespace sbsar {

auto Package::load_from_file(const std::string& path, bool instantiate) -> void
{
	url = path;
	auto file_data = std::vector<unsigned char>{};
	load_file(file_data, path, true, ctx->logger.get());

	descriptor = std::make_unique<sbs::PackageDesc>(file_data.data(), file_data.size());

	if (!descriptor->isValid()) {
		auto msg = fmt::format("Package [{}] is invalid", path);
		ctx->logger->error(msg);
		throw std::runtime_error(msg);
	}

	ctx->logger->debug("Loaded package from {}", path);

	const auto& pkg_graphs = descriptor->getGraphs();

	for (const auto& pkg_graph : descriptor->getGraphs()) {
		ctx->logger->debug("Loading graph: {}", pkg_graph.mPackageUrl);

		auto graph_url = std::string(pkg_graph.mPackageUrl);

		graph_url = graph_url.substr(6);

		auto& graph = graphs_container.emplace_back(pkg_graph);

		graph.ctx = ctx;
		graph.package_url = graph_url;

		graph.load_parameters();
		graph.load_outputs();
		graph.load_inputs();

		if (instantiate) {
			graph.instance = std::make_unique<sbs::GraphInstance>(pkg_graph);
			graph.link_instance();
		}
	}

	for (auto& graph : graphs_container) {
		graphs_map.try_emplace(graph.package_url, graph);
	}

}

Package::~Package()
{
	graphs_map.clear();
	graphs_container.clear();
	instances.clear();

	ctx->logger->debug("Unload package {}", url);
}

}

