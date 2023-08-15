#include "capi/sbsar.h"
#include "spdlog/sinks/base_sink.h"

namespace sbsar::capi {

std::unique_ptr<Context> ctx;

auto string_handle(const std::string& str) -> StringHandle {
	return StringHandle{str.data(), static_cast<uint32_t>(str.size())};
}

auto string_handle(const sbs::string& str) -> StringHandle {
	return StringHandle{str.data(), static_cast<uint32_t>(str.size())};
}

template <typename T>
auto get_parm_slider_step(Parameter* parm) -> float {
	auto descriptor = parm->descriptor_for<T>();
	if (!descriptor) return 0.f;
	return descriptor->mSliderStep;
}

template <typename T>
auto get_parm_slider_clamp(Parameter* parm) -> bool {
	auto descriptor = parm->descriptor_for<T>();
	if (!descriptor) return false;
	return descriptor->mSliderClamp;
}

extern "C" {

SBSAPI_EXPORT auto init_context() -> void {
	ctx = std::make_unique<Context>();
}

SBSAPI_EXPORT auto init_context_with_log(LogCallback log_callback) -> void {
	ctx = std::make_unique<Context>(log_callback);
}

SBSAPI_EXPORT auto shutdown() -> void {
	ctx.reset();
}

SBSAPI_EXPORT auto register_log_callback(LogCallback log_callback) -> void {
	ctx->add_log_callback(log_callback);
}

// Package
SBSAPI_EXPORT auto load_package(const char* filename) -> Package* {
	auto pkg = ctx->load_package(filename);
	if (pkg) {
		return &*pkg;
	}
	else {
		ctx->log_error(pkg.error().message);
		return nullptr;
	}
}

SBSAPI_EXPORT auto package_graph(Package* package, const char* id) -> Graph* {
	return package->graph(id);
}

SBSAPI_EXPORT auto package_num_graphs(Package* package) -> uint32_t {
	return package->graphs().size();
}

SBSAPI_EXPORT auto package_graphs(Package* package, Graph** graphs) -> void {
	for (auto&& [index, graph] : package->graphs() | vi::enumerate) {
		graphs[index] = &graph;
	}
}

// Graph
SBSAPI_EXPORT auto graph_url(Graph* graph) -> StringHandle {
	return string_handle(graph->url());
}

SBSAPI_EXPORT auto graph_label(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mLabel);
}

SBSAPI_EXPORT auto graph_description(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mDescription);
}

SBSAPI_EXPORT auto graph_category(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mCategory);
}

SBSAPI_EXPORT auto graph_keywords(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mKeywords);
}

SBSAPI_EXPORT auto graph_author(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mAuthor);
}

SBSAPI_EXPORT auto graph_author_url(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mAuthorUrl);
}

SBSAPI_EXPORT auto graph_user_tag(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mUserTag);
}

SBSAPI_EXPORT auto graph_type(Graph* graph) -> StringHandle {
	return string_handle(graph->descriptor()->mTypeStr);
}

SBSAPI_EXPORT auto graph_render(Graph* graph, int grab_results) -> void {
	graph->render(grab_results);
}

SBSAPI_EXPORT auto graph_parm(Graph* graph, const char* id) -> Parameter* {
	return graph->parm(id);
}

SBSAPI_EXPORT auto graph_input(Graph* graph, const char* id) -> Input* {
	return graph->input(id);
}

SBSAPI_EXPORT auto graph_output(Graph* graph, const char* id) -> Output* {
	return graph->output(id);
}

SBSAPI_EXPORT auto graph_num_parms(Graph* graph) -> uint32_t {
	return graph->parms().size();
}

SBSAPI_EXPORT auto graph_num_inputs(Graph* graph) -> uint32_t {
	return graph->inputs().size();
}

SBSAPI_EXPORT auto graph_num_outputs(Graph* graph) -> uint32_t {
	return graph->outputs().size();
}

SBSAPI_EXPORT auto graph_parms(Graph* graph, Parameter** parms) -> void {
	for (auto&& [index, parm] : graph->parms() | vi::enumerate) {
		parms[index] = &parm;
	}
}

SBSAPI_EXPORT auto graph_inputs(Graph* graph, Input** inputs) -> void {
	for (auto&& [index, input] : graph->inputs() | vi::enumerate) {
		inputs[index] = &input;
	}
}

SBSAPI_EXPORT auto graph_outputs(Graph* graph, Output** outputs) -> void {
	for (auto&& [index, output] : graph->outputs() | vi::enumerate) {
		outputs[index] = &output;
	}
}

// Parameters

SBSAPI_EXPORT auto parm_set_string(Parameter* parm, const char* value) -> void {
	parm->set(value);
}

SBSAPI_EXPORT auto parm_set_float(Parameter* parm, float* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_float2(Parameter* parm, sbs::Vec2Float* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_float3(Parameter* parm, sbs::Vec3Float* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_float4(Parameter* parm, sbs::Vec4Float* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_int(Parameter* parm, int* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_int2(Parameter* parm, sbs::Vec2Int* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_int3(Parameter* parm, sbs::Vec3Int* value) -> void {
	parm->set(*value);
}

SBSAPI_EXPORT auto parm_set_int4(Parameter* parm, sbs::Vec4Int* value) -> void {
	parm->set(*value);
}

#define DEF_PARM_GET_API_FUNCTION(API_FUNC_NAME, FUNC_NAME) \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_string(Parameter* parm, StringHandle* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<std::string>(); \
    if (!result) return; \
    (*value) = string_handle(*result); \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float(Parameter* parm, float* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<float>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float2(Parameter* parm, sbs::Vec2Float* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec2Float>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float3(Parameter* parm, sbs::Vec3Float* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec3Float>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float4(Parameter* parm, sbs::Vec4Float* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec4Float>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int(Parameter* parm, int* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<int>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int2(Parameter* parm, sbs::Vec2Int* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec2Int>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int3(Parameter* parm, sbs::Vec3Int* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec3Int>(); \
	if (!result) return; \
    (*value) = *result; \
} \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int4(Parameter* parm, sbs::Vec4Int* value) -> void \
{ \
    auto result = parm->FUNC_NAME##_as<sbs::Vec4Int>(); \
	if (!result) return; \
    (*value) = *result; \
} \

DEF_PARM_GET_API_FUNCTION(get, get)
DEF_PARM_GET_API_FUNCTION(default, default_value)
DEF_PARM_GET_API_FUNCTION(max, max_value)
DEF_PARM_GET_API_FUNCTION(min, min_value)

#undef DEF_PARM_GET_API_FUNCTION

SBSAPI_EXPORT auto parm_id(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mIdentifier);
}

SBSAPI_EXPORT auto parm_label(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mLabel);
}

SBSAPI_EXPORT auto parm_widget(Parameter* parm) -> ParameterWidget {
	return parm->widget();
}

SBSAPI_EXPORT auto parm_num_usages(Parameter* parm) -> uint32_t {
	return parm->descriptor()->mChannelsStr.size();
}

SBSAPI_EXPORT auto parm_usages(Parameter* parm, StringHandle* usages) -> void {
	for (auto&& [index, usage] : parm->descriptor()->mChannelsStr | vi::enumerate) {
		usages[index] = string_handle(usage);
	}
}

SBSAPI_EXPORT auto parm_is_visible(Parameter* parm) -> int {
	return parm->is_visible();
}

SBSAPI_EXPORT auto parm_type(Parameter* parm) -> ParameterType {
	return parm->type();
}

SBSAPI_EXPORT auto parm_group(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mGuiGroup);
}

SBSAPI_EXPORT auto parm_description(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mGuiDescription);
}

SBSAPI_EXPORT auto parm_visible_condition(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mGuiVisibleIf);
}

SBSAPI_EXPORT auto parm_user_tag(Parameter* parm) -> StringHandle {
	return string_handle(parm->descriptor()->mUserTag);
}

SBSAPI_EXPORT auto parm_num_component_labels(Parameter* parm) -> uint32_t {
	if (parm->descriptor()->isNumerical()) {
		auto descriptor = dynamic_cast<const sbs::InputDescNumericalBase*>(parm->descriptor());
		if (!descriptor) return 0;
		return descriptor->mGuiVecLabels.size();
	}
	return 1;
}

SBSAPI_EXPORT auto parm_slider_step(Parameter* parm) -> float {
	auto type = parm->type();
	switch (type) {
		case ParameterType::STRING:
			return 0.f;
		case ParameterType::FLOAT:
			return get_parm_slider_step<float>(parm);
		case ParameterType::FLOAT2:
			return get_parm_slider_step<sbs::Vec2Float>(parm);
		case ParameterType::FLOAT3:
			return get_parm_slider_step<sbs::Vec3Float>(parm);
		case ParameterType::FLOAT4:
			return get_parm_slider_step<sbs::Vec4Float>(parm);
		case ParameterType::INTEGER:
			return get_parm_slider_step<int>(parm);
		case ParameterType::INTEGER2:
			return get_parm_slider_step<sbs::Vec2Int>(parm);
		case ParameterType::INTEGER3:
			return get_parm_slider_step<sbs::Vec3Int>(parm);
		case ParameterType::INTEGER4:
			return get_parm_slider_step<sbs::Vec4Int>(parm);
		default:
			return 0.f;
	}
}

SBSAPI_EXPORT auto parm_slider_clamp(Parameter* parm) -> int {
	auto type = parm->type();
	switch (type) {
		case ParameterType::STRING:
			return 0;
		case ParameterType::FLOAT:
			return get_parm_slider_clamp<float>(parm);
		case ParameterType::FLOAT2:
			return get_parm_slider_clamp<sbs::Vec2Float>(parm);
		case ParameterType::FLOAT3:
			return get_parm_slider_clamp<sbs::Vec3Float>(parm);
		case ParameterType::FLOAT4:
			return get_parm_slider_clamp<sbs::Vec4Float>(parm);
		case ParameterType::INTEGER:
			return get_parm_slider_clamp<int>(parm);
		case ParameterType::INTEGER2:
			return get_parm_slider_clamp<sbs::Vec2Int>(parm);
		case ParameterType::INTEGER3:
			return get_parm_slider_clamp<sbs::Vec3Int>(parm);
		case ParameterType::INTEGER4:
			return get_parm_slider_clamp<sbs::Vec4Int>(parm);
		default:
			return 0;
	}
}

SBSAPI_EXPORT auto parm_num_choices(Parameter* parm) -> uint32_t {
	if (parm->type() != ParameterType::INTEGER) return 0;
	auto descriptor = parm->descriptor_for<int>();
	if (!descriptor) return 0;
	return descriptor->mEnumValues.size();
}

SBSAPI_EXPORT auto parm_choices(Parameter* parm, ParameterChoice* choices) -> void {
	if (parm->type() != ParameterType::INTEGER) return;
	auto descriptor = parm->descriptor_for<int>();
	if (!descriptor) return;

	for (auto&& [index, choice] : descriptor->mEnumValues | vi::enumerate) {
		auto& [value, name] = choice;
		choices[index].name = string_handle(name);
		choices[index].value = value;
	}
}

SBSAPI_EXPORT auto parm_component_labels(Parameter* parm, StringHandle* labels) -> void {
	if (parm->descriptor()->isNumerical()) {
		auto descriptor = dynamic_cast<const sbs::InputDescNumericalBase*>(parm->descriptor());
		if (!descriptor) return;
		for (auto&& [index, label] : descriptor->mGuiVecLabels | vi::enumerate) {
			labels[index] = string_handle(label);
		}
	}
	else if (parm->descriptor()->isString()) {
		auto descriptor = parm->descriptor_for<sbs::string>();
		if (!descriptor) return;
		labels[0] = string_handle(descriptor->mLabel);
	}
}

SBSAPI_EXPORT auto parm_label_true(Parameter* parm) -> StringHandle {
	auto type = parm->type();
	if (type == ParameterType::INTEGER) {
		auto descriptor = dynamic_cast<const sbs::InputDescNumerical<int>*>(parm->descriptor());
		if (descriptor) return string_handle(descriptor->mLabelTrue);
	}

	return StringHandle{nullptr, 0};

}

SBSAPI_EXPORT auto parm_label_false(Parameter* parm) -> StringHandle {
	auto type = parm->type();
	if (type == ParameterType::INTEGER) {
		auto descriptor = dynamic_cast<const sbs::InputDescNumerical<int>*>(parm->descriptor());
		if (descriptor) return string_handle(descriptor->mLabelFalse);
	}

	return StringHandle{nullptr, 0};
}

// Outputs
SBSAPI_EXPORT auto output_id(Output* output) -> StringHandle {
	return string_handle(output->descriptor()->mIdentifier);
}

SBSAPI_EXPORT auto output_label(Output* output) -> StringHandle {
	return string_handle(output->descriptor()->mLabel);
}

SBSAPI_EXPORT auto output_group(Output* output) -> StringHandle {
	return string_handle(output->descriptor()->mGroup);
}

SBSAPI_EXPORT auto output_visible_condition(Output* output) -> StringHandle {
	return string_handle(output->descriptor()->mGuiVisibleIf);
}

SBSAPI_EXPORT auto output_num_usages(Output* output) -> uint32_t {
	return output->descriptor()->mChannelsStr.size();
}

SBSAPI_EXPORT auto output_usages(Output* output, StringHandle* usages) -> void {
	for (auto&& [index, usage] : output->descriptor()->mChannelsStr | vi::enumerate) {
		usages[index] = string_handle(usage);
	}
}

SBSAPI_EXPORT auto output_data(Output* output) -> void* {
	return output->get_raw_data();
}

SBSAPI_EXPORT auto output_save(Output* output, const char* filename) -> void {
	output->save(filename);
}

SBSAPI_EXPORT auto output_format(Output* output, PixelFormat* format) -> void {
	(*format) = output->get_data_format();
}

SBSAPI_EXPORT auto output_size(Output* output, int* width, int* height) -> void {
	auto [w, h] = output->get_data_size();
	*width = w;
	*height = h;
}

SBSAPI_EXPORT auto output_override_size(Output* output, OutputResolution* resolution) -> void {
	output->override_format(*resolution);
}

SBSAPI_EXPORT auto output_override_format(Output* output, OutputFormatOverride* format) -> void {
	output->override_format(*format);
}

SBSAPI_EXPORT auto output_override_pixel_format(Output* output, PixelFormat* format) -> void {
	output->override_format(*format);
}

SBSAPI_EXPORT auto output_grab_result(Output* output) -> void {
	output->grab_result();
}

// Inputs
SBSAPI_EXPORT auto input_id(Input* input) -> StringHandle {
	return string_handle(input->descriptor()->mIdentifier);
}

SBSAPI_EXPORT auto input_label(Input* input) -> StringHandle {
	return string_handle(input->descriptor()->mLabel);
}

SBSAPI_EXPORT auto input_visible_condition(Input* input) -> StringHandle {
	return string_handle(input->descriptor()->mGuiVisibleIf);
}

SBSAPI_EXPORT auto input_is_visible(Input* input) -> int {
	return input->is_visible();
}

SBSAPI_EXPORT auto input_num_usages(Input* input) -> uint32_t {
	return input->descriptor()->mChannelsStr.size();
}

SBSAPI_EXPORT auto input_usages(Input* input, StringHandle* usages) -> void {
	for (auto&& [index, usage] : input->descriptor()->mChannelsStr | vi::enumerate) {
		usages[index] = string_handle(usage);
	}
}

SBSAPI_EXPORT auto
input_load_from_buffer(Input* input, void* data, const int* width, const int* height, PixelFormat* format) -> void {
	input->load_from_buffer(data, *width, *height, *format);
}

SBSAPI_EXPORT auto input_load_from_file(Input* input, const char* filename) -> void {
	input->load_from_file(filename);
}

}
}