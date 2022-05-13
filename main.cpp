#include "utils/common.h"
#include "sbsar/context.h"

const auto sbsar_path = "C:/Users/elovikov/OneDrive/sbs/Unity/ring.sbsar";


int main()
{
	spdlog::set_level(spdlog::level::debug);

	auto ctx = sbsar::Context();
	auto& pkg = ctx.load_package(sbsar_path);

	auto& graph = pkg.graph("ring");
	auto& parm = graph.parm("Radius");
	spdlog::info("max value: {}", parm.max_value_as<float>());
	parm.set(10);


	return 0;
}
