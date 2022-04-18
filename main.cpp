#include "utils/common.h"
#include "sbsar/Context.h"

const auto sbsar_path = "D:/graph.sbsar";

int main()
{
	spdlog::set_level(spdlog::level::debug);

	auto ctx = sbsar::Context();
	auto& pkg = ctx.load_package(sbsar_path, true);

	return 0;
}
