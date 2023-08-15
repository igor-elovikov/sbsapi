#include "utils/common.h"
#include "sbsar/context.h"

const auto sbsar_path = "C:/Users/elovikov/OneDrive/sbs/Unity/ring.sbsar";

int main() {
	add_to_env("PATH",
	  R"(;C:\Program Files\Adobe\Adobe Substance 3D Designer;C:\Program Files\Adobe\Adobe Substance 3D Designer\plugins\engines)");

	auto ctx = sbsar::Context();

	auto pkg = ctx.load_package(sbsar_path);

	pkg->graph("ring")->render();
	auto graph = pkg->graph("ring");
	graph->output("output")->save("D:/out.png");

	return 0;

}
