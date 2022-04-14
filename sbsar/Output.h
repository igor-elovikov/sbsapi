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

	[[nodiscard]] auto get_raw_data() const -> void*
	{
		if (!render_result) return nullptr;
		return render_result->get_raw_data();
	}

	[[nodiscard]] const auto& get_data_format() const
	{
		if (!render_result) return format;
		return render_result->format;
	}

	[[nodiscard]] auto get_data_size() const
	{
		if (!render_result) return std::make_pair(0, 0);
		return std::make_pair(render_result->width, render_result->height);
	}
};
}
