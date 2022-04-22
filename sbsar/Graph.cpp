#include "Graph.h"
#include "Context.h"

namespace sbsar {

auto Graph::link_instance() -> void
{
	if (!instance) return;
	auto& inputs = instance->getInputs();

	if (!sbs_descriptor) return;

	for (auto& input : inputs) {
		auto id = std::string(input->mDesc.mIdentifier);

		if (input->mDesc.isNumerical()) {
			if (has_parm(id)) {
				parms_map.at(id).instance = input;
			}
			else {
				spdlog::warn("Couldn't find descriptor for parameter [{}]", id);
			}
		}
		else if (input->mDesc.isImage()) {
			if (has_input(id)) {
				inputs_map.at(id).instance = dynamic_cast<sbs::InputInstanceImage*>(input);
			}
			else {
				spdlog::warn("Couldn't find descriptor for input [{}]", id);
			}
		}
	}

	auto& instance_outputs = instance->getOutputs();

	for (auto& instance_output : instance_outputs) {
		if (!instance_output->mDesc.isImage()) continue;
		auto id = std::string(instance_output->mDesc.mIdentifier);
		if (has_output(id)) {
			outputs_map.at(id).instance = instance_output;
		}
		else {
			spdlog::warn("Couldn't find output descriptor for {}", instance_output->mDesc.mIdentifier);
		}
	}
}

auto Graph::load_outputs() -> void
{
	if (!sbs_descriptor) return;

	auto& graph_outputs = sbs_descriptor->mOutputs;

	outputs_map.clear();
	outputs_container.clear();

	for (const auto& graph_output : graph_outputs) {
		auto id = std::string(graph_output.mIdentifier);
		auto& output = outputs_container.emplace_back(&graph_output);
		output.set_format_from_descriptor();
	}

	for (auto& output : outputs_container) {
		outputs_map.try_emplace(output.id(), output);
	}
}

auto Graph::load_parameters() -> void
{
	if (!sbs_descriptor) return;

	auto& inputs = sbs_descriptor->mInputs;

	parms_map.clear();
	parms_container.clear();

	for (const auto& input : inputs) {
		if (input->isImage()) continue;
		auto id = std::string(input->mIdentifier);
		auto& parm = parms_container.emplace_back(input);
	}

	for (auto& input : parms_container) {
		parms_map.try_emplace(input.id(), input);
	}

}

auto Graph::load_inputs() -> void
{
	if (!sbs_descriptor) return;

	auto& graph_inputs = sbs_descriptor->mInputs;

	inputs_map.clear();
	inputs_container.clear();

	for (const auto& graph_input : graph_inputs) {
		if (!graph_input->isImage()) continue;
		auto id = std::string(graph_input->mIdentifier);
		auto& input = inputs_container.emplace_back(dynamic_cast<const sbs::InputDescImage*>(graph_input));
	}

	for (auto& input : inputs_container) {
		inputs_map.try_emplace(input.id(), input);
	}

}

auto Graph::render(bool grab_results) -> void
{
	if (!ctx->renderer) {
		spdlog::debug("Rendering graph [{}] cancelled: renderer is not enabled in context", package_url);
		return;
	}

	spdlog::debug("Rendering graph [{}]", package_url);
	ctx->renderer->push(*instance);
	ctx->renderer->run();
	spdlog::debug("Done rendering");

	if (grab_results) {
		rn::for_each(outputs_map | vi::values, &Output::grab_result);
	}
}

auto Graph::set_resolution(OutputSize resolution_x, OutputSize resolution_y) -> void
{
	if (!has_parm(size_parm_name) || resolution_x == OutputSize::NONE || resolution_y == OutputSize::NONE) return;
	auto& size_parm = parm(size_parm_name);

	size_parm.set(sbs::Vec2Int(
	  static_cast<int>(resolution_x),
	  static_cast<int>(resolution_y)
	));
}

}
