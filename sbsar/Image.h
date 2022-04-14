#pragma once

#include "common.h"
#include "types.h"

namespace sbsar {

class Image {

	friend class Output;
	friend class Input;

	std::vector<unsigned char> data;

	sbs::unique_ptr<sbs::RenderResultBase> render_result;
	sbs::RenderResultImage* rendered_image = nullptr;

	bool is_rendered = false;

	[[nodiscard]] inline auto get_raw_data() const -> void*
	{
		return is_rendered ? rendered_image->getTexture().buffer : (void*)data.data();
	}

public:
	Image() = default;
	explicit Image(sbs::OutputInstance* output_instance);
	explicit Image(const std::string& filename)
	{
		load(filename);
	}

	ImageFormat format{};

	int width = 0;
	int height = 0;

	auto load(const std::string& filename) -> void;
	auto save(const std::string& filename) const -> void;
};

}