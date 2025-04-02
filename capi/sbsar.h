#pragma once

#include <cstdint>

#include "sbsapi_export.h"
#include "common.h"
#include "sbsar/package.h"
#include "sbsar/context.h"

namespace sbsar {
enum class ParameterType;
enum class ParameterWidget;
}

namespace sbsar::capi {

struct StringHandle {
	const char* data;
	uint32_t size;
};

struct ParameterChoice {
	StringHandle name{};
	int value{};
};

enum class ApiCallResult : uint32_t {
	Success = 0,
	Failure
};

extern "C" {

SBSAPI_EXPORT auto init_context() -> void;
SBSAPI_EXPORT auto shutdown() -> void;
SBSAPI_EXPORT auto register_log_callback(LogCallback log_callback) -> void;

// Package
SBSAPI_EXPORT auto load_package(const char* filename) -> Package*;
SBSAPI_EXPORT auto package_graph(Package* package, const char* id) -> Graph*;
SBSAPI_EXPORT auto package_num_graphs(Package* package) -> uint32_t;
SBSAPI_EXPORT auto package_graphs(Package* package, Graph** graphs) -> void;

// Graph
SBSAPI_EXPORT auto graph_url(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_label(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_description(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_category(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_keywords(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_author(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_author_url(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_user_tag(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_type(Graph* graph) -> StringHandle;
SBSAPI_EXPORT auto graph_render(Graph* graph, int grab_results) -> void;
SBSAPI_EXPORT auto graph_parm(Graph* graph, const char* id) -> Parameter*;
SBSAPI_EXPORT auto graph_input(Graph* graph, const char* id) -> Input*;
SBSAPI_EXPORT auto graph_output(Graph* graph, const char* id) -> Output*;
SBSAPI_EXPORT auto graph_num_parms(Graph* graph) -> uint32_t;
SBSAPI_EXPORT auto graph_num_inputs(Graph* graph) -> uint32_t;
SBSAPI_EXPORT auto graph_num_outputs(Graph* graph) -> uint32_t;
SBSAPI_EXPORT auto graph_parms(Graph* graph, Parameter** parms) -> void;
SBSAPI_EXPORT auto graph_inputs(Graph* graph, Input** inputs) -> void;
SBSAPI_EXPORT auto graph_outputs(Graph* graph, Output** outputs) -> void;

// Parameters

SBSAPI_EXPORT auto parm_set_string(Parameter* parm, const char* value) -> void;
SBSAPI_EXPORT auto parm_set_float(Parameter* parm, float* value) -> void;
SBSAPI_EXPORT auto parm_set_float2(Parameter* parm, sbs::Vec2Float* value) -> void;
SBSAPI_EXPORT auto parm_set_float3(Parameter* parm, sbs::Vec3Float* value) -> void;
SBSAPI_EXPORT auto parm_set_float4(Parameter* parm, sbs::Vec4Float* value) -> void;
SBSAPI_EXPORT auto parm_set_int(Parameter* parm, int* value) -> void;
SBSAPI_EXPORT auto parm_set_int2(Parameter* parm, sbs::Vec2Int* value) -> void;
SBSAPI_EXPORT auto parm_set_int3(Parameter* parm, sbs::Vec3Int* value) -> void;
SBSAPI_EXPORT auto parm_set_int4(Parameter* parm, sbs::Vec4Int* value) -> void;

#define DEF_PARM_GET_API_FUNCTION(API_FUNC_NAME, FUNC_NAME) \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_string(Parameter* parm, StringHandle* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float(Parameter* parm, float* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float2(Parameter* parm, sbs::Vec2Float* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float3(Parameter* parm, sbs::Vec3Float* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_float4(Parameter* parm, sbs::Vec4Float* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int(Parameter* parm, int* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int2(Parameter* parm, sbs::Vec2Int* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int3(Parameter* parm, sbs::Vec3Int* value) -> void; \
SBSAPI_EXPORT auto parm_##API_FUNC_NAME##_int4(Parameter* parm, sbs::Vec4Int* value) -> void; \

DEF_PARM_GET_API_FUNCTION(get, get)
DEF_PARM_GET_API_FUNCTION(default, default_value)
DEF_PARM_GET_API_FUNCTION(max, max_value)
DEF_PARM_GET_API_FUNCTION(min, min_value)

#undef DEF_PARM_GET_API_FUNCTION

SBSAPI_EXPORT auto parm_id(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_label(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_widget(Parameter* parm) -> ParameterWidget;
SBSAPI_EXPORT auto parm_num_usages(Parameter* parm) -> uint32_t;
SBSAPI_EXPORT auto parm_usages(Parameter* parm, StringHandle* usages) -> void;
SBSAPI_EXPORT auto parm_is_visible(Parameter* parm) -> int;
SBSAPI_EXPORT auto parm_type(Parameter* parm) -> ParameterType;
SBSAPI_EXPORT auto parm_group(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_description(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_visible_condition(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_user_tag(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_num_component_labels(Parameter* parm) -> uint32_t;
SBSAPI_EXPORT auto parm_slider_step(Parameter* parm) -> float;
SBSAPI_EXPORT auto parm_slider_clamp(Parameter* parm) -> int;
SBSAPI_EXPORT auto parm_num_choices(Parameter* parm) -> uint32_t;
SBSAPI_EXPORT auto parm_choices(Parameter* parm, ParameterChoice* choices) -> void;
SBSAPI_EXPORT auto parm_component_labels(Parameter* parm, StringHandle* labels) -> void;
SBSAPI_EXPORT auto parm_label_true(Parameter* parm) -> StringHandle;
SBSAPI_EXPORT auto parm_label_false(Parameter* parm) -> StringHandle;

// Outputs
SBSAPI_EXPORT auto output_id(Output* output) -> StringHandle;
SBSAPI_EXPORT auto output_label(Output* output) -> StringHandle;
SBSAPI_EXPORT auto output_group(Output* output) -> StringHandle;
SBSAPI_EXPORT auto output_visible_condition(Output* output) -> StringHandle;
SBSAPI_EXPORT auto output_num_usages(Output* output) -> uint32_t;
SBSAPI_EXPORT auto output_usages(Output* output, StringHandle* usages) -> void;
SBSAPI_EXPORT auto output_data(Output* output) -> void*;
SBSAPI_EXPORT auto output_save(Output* output, const char* filename) -> void;
SBSAPI_EXPORT auto output_format(Output* output, PixelFormat* format) -> void;
SBSAPI_EXPORT auto output_size(Output* output, int* width, int* height) -> void;
SBSAPI_EXPORT auto output_override_size(Output* output, OutputResolution* resolution) -> void;
SBSAPI_EXPORT auto output_override_format(Output* output, OutputFormatOverride* format) -> void;
SBSAPI_EXPORT auto output_override_pixel_format(Output* output, PixelFormat* format) -> void;
SBSAPI_EXPORT auto output_grab_result(Output* output) -> void;

// Inputs
SBSAPI_EXPORT auto input_id(Input* input) -> StringHandle;
SBSAPI_EXPORT auto input_label(Input* input) -> StringHandle;
SBSAPI_EXPORT auto input_visible_condition(Input* input) -> StringHandle;
SBSAPI_EXPORT auto input_is_visible(Input* input) -> int;
SBSAPI_EXPORT auto input_num_usages(Input* input) -> uint32_t;
SBSAPI_EXPORT auto input_usages(Input* input, StringHandle* usages) -> void;
SBSAPI_EXPORT auto
input_load_from_buffer(Input* input, void* data, const int* width, const int* height, PixelFormat* format) -> void;
SBSAPI_EXPORT auto input_load_from_file(Input* input, const char* filename) -> void;
}

}
