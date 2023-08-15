//
// Created by elovikov on 30/03/2022.
//

#include "package.h"
#include "context.h"

namespace sbsar {

Package::~Package()
{
	graphs_map.clear();
	graphs_container.clear();
	instances.clear();

	ctx->log_debug("Unload package {}", url);
}

}

