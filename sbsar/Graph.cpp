#include "Graph.h"
#include "Context.h"

namespace sbsar {

auto Graph::link_instance() -> void
{
	if (!instance) return;
	auto inputs = instance->getInputs();

	if (!descriptor) return;

	for (auto& input : inputs) {
		auto id = std::string(input->mDesc.mIdentifier);

		if (input->mDesc.isNumerical()) {
			if (has_parm(id)) {
				parms_map[id].instance = input;
			}
			else {
				spdlog::warn("Couldn't find descriptor for parameter [{}]", id);
			}
		}
		else if (input->mDesc.isImage()) {
			if (has_input(id)) {
				inputs_map[id].instance = dynamic_cast<sbs::InputInstanceImage*>(input);
			}
			else {
				spdlog::warn("Couldn't find descriptor for input [{}]", id);
			}
		}
	}

	auto instance_outputs = instance->getOutputs();

	for (auto& instance_output : instance_outputs) {
		if (!instance_output->mDesc.isImage()) continue;
		auto id = std::string(instance_output->mDesc.mIdentifier);
		if (has_output(id)) {
			outputs_map[id].instance = instance_output;
		}
		else {
			spdlog::warn("Couldn't find output descriptor for {}", instance_output->mDesc.mIdentifier);
		}
	}
}

auto Graph::load_outputs() -> void
{
	if (!descriptor) return;

	auto graph_outputs = descriptor->mOutputs;

	outputs_map.clear();
	output_names_ordered.clear();

	for (auto& graph_output : graph_outputs) {
		auto id = std::string(graph_output.mIdentifier);

		output_names_ordered.push_back(id);
		auto& output = outputs_map[id];

		output.instance = nullptr;
		output.sbs_descriptor = graph_output;

		output.label = graph_output.mLabel;
		output.usages = graph_output.mChannelsStr | rn::to<std::vector<std::string>>;

		output.set_format_from_descriptor();
	}
}

auto Graph::load_parameters() -> void
{
	if (!descriptor) return;

	auto inputs = descriptor->mInputs;

	parms_map.clear();
	parm_names_ordered.clear();

	for (auto& input : inputs) {
		if (input->isImage()) continue;
		auto id = std::string(input->mIdentifier);

		parm_names_ordered.push_back(id);
		auto& parm = parms_map[id];

		parm.instance = nullptr;
		parm.sbs_descriptor = input;
	}
}

auto Graph::load_inputs() -> void
{
	if (!descriptor) return;

	auto graph_inputs = descriptor->mInputs;

	inputs_map.clear();
	input_names_ordered.clear();

	for (auto& graph_input : graph_inputs) {
		if (!graph_input->isImage()) continue;
		auto id = std::string(graph_input->mIdentifier);

		input_names_ordered.push_back(id);
		auto& input = inputs_map[id];

		input.instance = nullptr;
		input.sbs_descriptor = dynamic_cast<const sbs::InputDescImage*>(graph_input);

		input.label = input.sbs_descriptor->mLabel;
		input.user_tag = input.sbs_descriptor->mUserTag;
	}
}

auto Graph::render(bool grab_results) -> void
{
	spdlog::debug("Rendering graph [{}]", url);
	ctx->renderer.push(*instance);
	ctx->renderer.run();
	spdlog::debug("Done rendering");

	if (grab_results) {
		rn::for_each(outputs_map | vi::values, &Output::grab_result);
	}
}

auto Graph::set_resolution(Resolution resolution_x, Resolution resolution_y) -> void
{
	if (!has_parm(size_parm_name)) return;
	auto size_parm = parm(size_parm_name);

	size_parm.set(sbs::Vec2Int(
	  static_cast<int>(resolution_x),
	  static_cast<int>(resolution_y)
	));
}

}
