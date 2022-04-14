#pragma once

#include "common.h"
#include "Parameter.h"
#include "Output.h"
#include "Input.h"

namespace sbsar {

class Context;

class Graph {

	Context* ctx = nullptr;

	std::unordered_map<std::string, Parameter> parms_map;
	std::vector<std::string> parm_names_ordered;

	std::unordered_map<std::string, Output> outputs_map;
	std::vector<std::string> output_names_ordered;

	std::unordered_map<std::string, Input> inputs_map;
	std::vector<std::string> input_names_ordered;

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
	std::shared_ptr<sbs::GraphDesc> descriptor;

	Graph() = default; // NOTE: for easier map (any workaround apart emplace?)
	~Graph()
	{
		if (instance) {
			spdlog::debug("Unload instanced graph: {}", url);
		}
		else {
			spdlog::debug("Unload graph without instance: {}", url);
		}
	}
	Graph& operator=(Graph&& other) = default;
	explicit Graph(const sbs::GraphDesc& graph_descriptor) { }

	[[nodiscard]] auto label() const { return std::string(descriptor->mLabel); }
	[[nodiscard]] auto description() const { return std::string(descriptor->mDescription); }
	[[nodiscard]] auto has_parm(const std::string& parm_id) const -> bool { return parms_map.contains(parm_id); }
	[[nodiscard]] auto& parm(const std::string& parm_id) { return parms_map[parm_id]; }
	[[nodiscard]] auto has_output(const std::string& output_id) const -> bool
	{
		return outputs_map.contains(output_id);
	}
	[[nodiscard]] auto& output(const std::string& output_id) { return outputs_map[output_id]; }
	[[nodiscard]] auto has_input(const std::string& output_id) const -> bool { return inputs_map.contains(output_id); }
	[[nodiscard]] auto& input(const std::string& output_id) { return inputs_map[output_id]; }
	[[nodiscard]] const auto& parms() const { return parm_names_ordered; }
	[[nodiscard]] const auto& outputs() const { return output_names_ordered; }
	[[nodiscard]] const auto& inputs() const { return input_names_ordered; }

	auto render(bool grab_results = true) -> void;
	auto set_resolution(Resolution resolution) -> void { set_resolution(resolution, resolution); }
	auto set_resolution(Resolution resolution_x, Resolution resolution_y) -> void;
};

}


