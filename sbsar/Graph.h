#pragma once

#include "common.h"
#include "Parameter.h"
#include "Output.h"
#include "Input.h"

namespace sbsar {

class Context;

class Graph {

	Context* ctx = nullptr;

	std::vector<Parameter> parms_container;
	std::unordered_map<std::string, Parameter*> parms_map;

	std::vector<Output> outputs_container;
	std::unordered_map<std::string, Output*> outputs_map;


	std::vector<Input> inputs_container;
	std::unordered_map<std::string, Input*> inputs_map;


	auto load_parameters() -> void;
	auto load_outputs() -> void;
	auto load_inputs() -> void;
	auto link_instance() -> void;

public:
	friend class Package;

	static constexpr auto size_parm_name = "$outputsize";
	static constexpr auto randomseed_parm_name = "$randomseed";

	std::string url;

	std::unique_ptr<sbs::GraphInstance> instance;
	const sbs::GraphDesc* descriptor = nullptr;

	Graph() = delete;
	Graph(Graph&) = delete;
	Graph(Graph&&) = default;
	explicit Graph(const sbs::GraphDesc& graph_descriptor) { descriptor = &graph_descriptor; }

	~Graph()
	{
		parms_map.clear();
		parms_container.clear();

		outputs_map.clear();
		outputs_container.clear();

		inputs_map.clear();
		inputs_container.clear();

		if (instance) {
			spdlog::debug("Unload instanced graph: {}", url);
			//instance.reset();
		}
		else {
			spdlog::debug("Unload graph without instance: {}", url);
		}
	}

	[[nodiscard]] auto label() const { return std::string(descriptor->mLabel); }
	[[nodiscard]] auto description() const { return std::string(descriptor->mDescription); }
	[[nodiscard]] auto has_parm(const std::string& parm_id) const -> bool { return parms_map.contains(parm_id); }
	[[nodiscard]] auto& parm(const std::string& parm_id) { return *parms_map.at(parm_id); }
	[[nodiscard]] auto has_output(const std::string& output_id) const -> bool
	{
		return outputs_map.contains(output_id);
	}
	[[nodiscard]] auto& output(const std::string& output_id) { return *outputs_map.at(output_id); }
	[[nodiscard]] auto has_input(const std::string& output_id) const -> bool { return inputs_map.contains(output_id); }
	[[nodiscard]] auto& input(const std::string& output_id) { return *inputs_map.at(output_id); }
	[[nodiscard]] const auto& parms() const { return parms_container; }
	[[nodiscard]] const auto& outputs() const { return outputs_container; }
	[[nodiscard]] const auto& inputs() const { return inputs_container; }

	auto render(bool grab_results = true) -> void;
	auto set_resolution(OutputSize resolution) -> void { set_resolution(resolution, resolution); }
	auto set_resolution(OutputSize resolution_x, OutputSize resolution_y) -> void;
};

}


