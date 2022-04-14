#pragma once

#include "common.h"
#include "Image.h"

namespace sbsar {

// currently, only image outputs_map
class Output {

	friend class Graph;

	sbs::OutputDesc sbs_descriptor;
	sbs::OutputInstance* instance = nullptr;

	std::unique_ptr<Image> render_result;

	auto set_format_from_descriptor()
	{
		format = ImageFormat((SubstancePixelFormat)sbs_descriptor.mFormat);
	}

public:

	const auto& descriptor() { return sbs_descriptor; }

	ImageFormat format{};

	std::string label;
	std::vector<std::string> usages;

	Output() = default;

	auto grab_result() -> void;
	auto save(const std::string& filename) -> void;
};
}
