#pragma once

namespace sbsar {

class Input {
	friend class Graph;

	const sbs::InputDescImage* sbs_descriptor = nullptr;
	sbs::InputInstanceImage* instance = nullptr;

	sbs::InputImage::SPtr input_image;

public:
	Input() = default;

	std::string label;
	std::vector<std::string> usages;
	std::string user_tag;

	[[nodiscard]] auto is_color() const { return sbs_descriptor->mIsColor; }
	[[nodiscard]] auto is_floating_point() const { return sbs_descriptor->mIsFPFormat; }

	auto load_from_file(const std::string& filename) -> void;
	auto load_from_buffer(void* data, int width, int height, PixelFormat format) -> void;
};

}