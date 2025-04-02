#include "utils/common.h"
#include "sbsar/context.h"


const auto sbsar_path = "C:/Users/elovikov/OneDrive/sbs/Unity/ring.sbsar";

struct S {
	int a;
};



int main() {

	auto s = std::optional<S*>{};


	auto ctx = sbsar::Context();

	auto pkg = ctx.load_package(sbsar_path);

	pkg->graph("ring")->render();
	auto graph = pkg->graph("ring");
	graph->output("output")->save("D:/out.png");

	if (const auto g = pkg->graph("ring"); g != nullptr) {

	}

	graph->parm("test")->set(10);

	return 0;

}
