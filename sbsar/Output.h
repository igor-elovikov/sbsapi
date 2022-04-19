#pragma once

#include "common.h"
#include "Image.h"

namespace sbsar {

// currently, only image outputs
class Output {

	friend class Graph;

	const sbs::OutputDesc* sbs_descriptor;
	sbs::OutputInstance* instance = nullptr;

	std::unique_ptr<Image> render_result;

	auto set_format_from_descriptor()
	{
		format = PixelFormat((SubstancePixelFormat)sbs_descriptor->mFormat);
	}

public:

	const auto& descriptor() { return sbs_descriptor; }

	PixelFormat format{};

	Output() = delete;
	Output(Output&) = delete;
	Output(Output&&) = default;
	explicit Output(const sbs::OutputDesc* desc) { sbs_descriptor = desc; }

	[[nodiscard]] auto id() const { return static_cast<std::string>(sbs_descriptor->mIdentifier); }
	[[nodiscard]] auto label() const { return static_cast<std::string>(sbs_descriptor->mLabel); }

	auto grab_result() -> void;
	auto save(const std::string& filename) -> void;
	auto override_format(const OutputFormatOverride& format_override) -> void;
	auto override_format(const OutputResolution& resolution) -> void;
	auto override_format(const PixelFormat& pixel_format) -> void;

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
