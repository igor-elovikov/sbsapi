#include "context.h"

namespace sbsar {

auto Context::load_package(const std::string& filename, bool instantiate) -> result<Package*> {

	logger->debug("Loading package from {}...", filename);
	if (auto pkg = packages.find(filename); pkg != packages.end()) {
		return pkg->second;
	}

	auto pkg = Package();
	pkg.ctx = this;

	pkg.url = filename;


	auto file = load_file<std::vector<unsigned char>>(filename, true);

	if (!file) {
		return file.error();
	}

	auto& file_data = *file;

	pkg.descriptor = std::make_unique<sbs::PackageDesc>(file_data.data(), file_data.size());

	if (!pkg.descriptor->isValid()) {
		return error(fmt::format("Package [{}] is invalid", filename));
	}

	logger->debug("Loaded package from {}", filename);

	for (const auto& pkg_graph : pkg.descriptor->getGraphs()) {
		logger->debug("Loading graph: {}", pkg_graph.mPackageUrl);

		auto graph_url = std::string(pkg_graph.mPackageUrl);

		graph_url = graph_url.substr(6);

		auto& graph = pkg.graphs_container.emplace_back(pkg_graph);

		graph.ctx = pkg.ctx;
		graph.renderer = renderer.get();
		graph.package_url = graph_url;

		graph.load_parameters();
		graph.load_outputs();
		graph.load_inputs();

		if (instantiate) {
			graph.instance = std::make_unique<sbs::GraphInstance>(pkg_graph);
			graph.link_instance();
		}
	}

	for (auto& graph : pkg.graphs_container) {
		pkg.graphs_map.try_emplace(graph.package_url, graph);
	}

	auto [new_package, result] = packages.emplace(filename, std::move(pkg));
	return new_package->second;
}

Context::Context(bool enable_renderer) {
	init_logging();
	log_info("Initialize SBSAR Context. Renderer enabled: {}", enable_renderer);
	if (enable_renderer) {
		renderer = std::make_unique<sbs::Renderer>();
		auto version = renderer->getCurrentVersion();
		log_info("Initialized renderer: v{}.{}", version.versionMajor, version.versionMinor);
	}
}

Context::Context(LogCallback log_callback, bool enable_renderer) {
	init_logging();
	add_log_callback(log_callback);
	log_info("Initialize SBSAR Context. Renderer enabled: {}", enable_renderer);
	if (enable_renderer) {
		renderer = std::make_unique<sbs::Renderer>();
		auto version = renderer->getCurrentVersion();
		log_info("Initialized renderer: v{}.{}", version.versionMajor, version.versionMinor);
	}
}

}