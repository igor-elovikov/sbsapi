//
// Created by elovikov on 07/04/2022.
//

#include "sbsar.h"

#include "sbsar/Context.h"
#include "sbsar/Package.h"

#include <spdlog/spdlog.h>

static auto graph_id = 0u;

struct APIContext {
	static std::map<std::string, std::unique_ptr<sbsar::Package>> loaded_packages;
};

std::unique_ptr<APIContext> ctx;


void sbsar_init_context()
{
	spdlog::info("Initialize API context");
	ctx = std::make_unique<APIContext>();
	graph_id = 0u;
}

void sbsar_shutdown()
{
	spdlog::info("Shutdown API");
	ctx.reset();
}

void sbsar_graph_render(sbsar::Graph* graph)
{
	graph->render();
}

void sbsar_graph_set_value_float(sbsar::Graph* graph, const char* parm_name, float value)
{
	if (graph->has_parm(parm_name)) {
		graph->parm(parm_name).set(value);
	}
}