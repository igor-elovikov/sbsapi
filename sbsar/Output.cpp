#include "Output.h"

namespace sbsar {

auto Output::grab_result() -> void
{
	if (!instance) return;
	render_result = std::make_unique<Image>(instance);
}

auto Output::save(const std::string& filename) -> void
{
	if (!render_result) return;
	render_result->save(filename);
}

}