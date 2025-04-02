#pragma once

#include "../utils/common.h"
#include "parameter.h"
#include "output.h"
#include "input.h"

namespace sbsar {

class Context;

class Graph {

	Context* ctx = nullptr;
	sbs::Renderer* renderer = nullptr;

	std::vector<Parameter> parms_container;
	std::unordered_map<std::string, Parameter&> parms_map;

	std::vector<Output> outputs_container;
	std::unordered_map<std::string, Output&> outputs_map;

	std::vector<Input> inputs_container;
	std::unordered_map<std::string, Input&> inputs_map;

	std::unique_ptr<sbs::GraphInstance> instance;
	const sbs::GraphDesc* sbs_descriptor = nullptr;

	auto load_parameters() -> void;
	auto load_outputs() -> void;
	auto load_inputs() -> void;
	auto link_instance() -> void;

public:
	std::string package_url;
	friend class Package;
	friend class Context;

	static constexpr auto size_parm_name = "$outputsize";
	static constexpr auto randomseed_parm_name = "$randomseed";
	[[nodiscard]] auto descriptor() const { return sbs_descriptor; }

	Graph() = default;
	Graph(const Graph&) = delete;
	Graph(Graph&&) = default;
	explicit Graph(const sbs::GraphDesc& graph_descriptor) { sbs_descriptor = &graph_descriptor; }

	[[nodiscard]] const auto& url() const { return package_url; }
	[[nodiscard]] auto label() const { return std::string(sbs_descriptor->mLabel); }
	[[nodiscard]] auto description() const { return std::string(sbs_descriptor->mDescription); }
	[[nodiscard]] auto category() const { return std::string(sbs_descriptor->mCategory); }
	[[nodiscard]] auto keywords() const { return std::string(sbs_descriptor->mKeywords); }
	[[nodiscard]] auto author() const { return std::string(sbs_descriptor->mAuthor); }
	[[nodiscard]] auto author_url() const { return std::string(sbs_descriptor->mAuthorUrl); }
	[[nodiscard]] auto user_tag() const { return std::string(sbs_descriptor->mUserTag); }
	[[nodiscard]] auto type() const { return std::string(sbs_descriptor->mTypeStr); }

	[[nodiscard]] auto& parms() { return parms_container; }
	[[nodiscard]] auto& outputs() { return outputs_container; }
	[[nodiscard]] auto& inputs() { return inputs_container; }

	[[nodiscard]] auto has_parm(const std::string& parm_id) const -> bool {
		return parms_map.find(parm_id) != parms_map.end();
	}

	[[nodiscard]] auto parm(const std::string& parm_id) -> Parameter* {
		if (auto parm = parms_map.find(parm_id); parm != parms_map.end()) {
			return &parm->second;
		}
		else {
			return nullptr;
		}
	}

	[[nodiscard]] auto has_output(const std::string& output_id) const -> bool {
		return outputs_map.find(output_id) != outputs_map.end();
	}

	[[nodiscard]] auto output(const std::string& output_id) -> Output* {
		if (auto output = outputs_map.find(output_id); output != outputs_map.end()) {
			return &output->second;
		}
		return nullptr;
	}

	[[nodiscard]] auto has_input(const std::string& output_id) const -> bool {
		return inputs_map.find(output_id) != inputs_map.end();
	}

	[[nodiscard]] auto input(const std::string& input_id) -> Input* {
		if (auto input = inputs_map.find(input_id); input != inputs_map.end()) {
			return &input->second;
		}
		return nullptr;
	}

	auto render(bool grab_results = true) -> void;
	auto set_resolution(OutputSize resolution) -> void { set_resolution(resolution, resolution); }
	auto set_resolution(OutputResolution resolution) -> void { set_resolution(resolution.width, resolution.height); }
	auto set_resolution(OutputSize resolution_x, OutputSize resolution_y) -> void;
};

}


