#include "utils/common.h"
#include "sbsar/Context.h"
#include "sbsar/Package.h"

const auto sbsar_path = "C:/Users/elovikov/OneDrive/sbs/Unity/ring.sbsar";

int main()
{
	spdlog::set_level(spdlog::level::debug);

	auto ctx = sbsar::Context();
	auto& pkg = ctx.load_package(sbsar_path);

	auto& graph = pkg.graphs["ring"];
	auto& out = pkg.graphs["ring"].output("output");

	graph.set_resolution(sbsar::Resolution::x1024);
	auto v = graph.parm("Radius").get();

	graph.parm("Radius").set(0.4f);
	graph.parm("Width").set("test");
	graph.render();
	out.save("D:/test.png");

	pkg.graphs["blur"].input("input").load_from_file("D:/test.png");
	pkg.graphs["blur"].render();
	pkg.graphs["blur"].output("output").save("D:/blurred.png");

	return 0;
}
