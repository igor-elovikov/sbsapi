#pragma once
#include "sbsapi_export.h"
#include "common.h"
#include "sbsar/Package.h"
#include "sbsar/Context.h"

std::unique_ptr<sbsar::Context> ctx;

extern "C" {

struct StringHandle {
	const char* data;
	uint32_t size;
};

SBSAPI_EXPORT auto sbsar_init_context() -> void
{
	ctx = std::make_unique<sbsar::Context>();
}

SBSAPI_EXPORT auto sbsar_shutdown() -> void
{
	ctx.reset();
}


// Package
SBSAPI_EXPORT auto sbsar_load_package(const char* filename) -> sbsar::Package*
{
	return &ctx->load_package(filename);
}

SBSAPI_EXPORT auto sbsar_package_get_graph(sbsar::Package* package, const char* id) -> sbsar::Graph*
{
	return &package->graph(id);
}

SBSAPI_EXPORT auto sbsar_package_get_num_graphs(sbsar::Package* package) -> uint32_t
{
	return package->graphs().size();
}

SBSAPI_EXPORT auto sbsar_package_get_graphs(sbsar::Package* package, sbsar::Graph* graphs) -> void
{
}

// Graph
SBSAPI_EXPORT auto sbsar_graph_get_id(sbsar::Graph* graph, StringHandle* id) -> void;
SBSAPI_EXPORT auto sbsar_graph_get_label(sbsar::Graph* graph, StringHandle* label) -> void;

SBSAPI_EXPORT auto sbsar_graph_render(sbsar::Graph* graph) -> void;
SBSAPI_EXPORT auto sbsar_graph_get_parm(sbsar::Graph* graph, const char* id) -> sbsar::Parameter*;
SBSAPI_EXPORT auto sbsar_graph_get_input(sbsar::Graph* graph, const char* id) -> sbsar::Input*;
SBSAPI_EXPORT auto sbsar_graph_get_output(sbsar::Graph* graph, const char* id) -> sbsar::Output*;

// Parameters
SBSAPI_EXPORT auto sbsar_parm_get_label(sbsar::Parameter* parm, StringHandle* label) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_widget(sbsar::Parameter* parm, sbsar::ParameterWidget* widget) -> void;

SBSAPI_EXPORT auto sbsar_parm_set_value_string(sbsar::Parameter* parm, const char* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_float(sbsar::Parameter* parm, float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_float2(sbsar::Parameter* parm, sbs::Vec2Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_float3(sbsar::Parameter* parm, sbs::Vec3Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_float4(sbsar::Parameter* parm, sbs::Vec4Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_int(sbsar::Parameter* parm, int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_int2(sbsar::Parameter* parm, sbs::Vec2Int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_int3(sbsar::Parameter* parm, sbs::Vec3Int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_set_value_int4(sbsar::Parameter* parm, sbs::Vec4Int* value) -> void;

SBSAPI_EXPORT auto sbsar_parm_get_value_string(sbsar::Parameter* parm, StringHandle* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_float(sbsar::Parameter* parm, float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_float2(sbsar::Parameter* parm, sbs::Vec2Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_float3(sbsar::Parameter* parm, sbs::Vec3Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_float4(sbsar::Parameter* parm, sbs::Vec4Float* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_int(sbsar::Parameter* parm, int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_int2(sbsar::Parameter* parm, sbs::Vec2Int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_int3(sbsar::Parameter* parm, sbs::Vec3Int* value) -> void;
SBSAPI_EXPORT auto sbsar_parm_get_value_int4(sbsar::Parameter* parm, sbs::Vec4Int* value) -> void;

SBSAPI_EXPORT auto sbsar_shutdown() -> void;

}