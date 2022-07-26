#pragma once

#include "common.h"
#include <substance/framework/visibleif.h>

namespace sbsar {

class Input {
	friend class Graph;
	spdlog::logger* logger = nullptr;

	const sbs::InputDescImage* sbs_descriptor = nullptr;
	sbs::InputInstanceImage* instance = nullptr;
	sbs::InputImage::SPtr input_image;

public:
	Input() = delete;
	Input(Input&) = delete;
	Input(Input&&) = default;
	explicit Input(const sbs::InputDescImage* desc) { sbs_descriptor = desc; }

	[[nodiscard]] auto descriptor() const { return sbs_descriptor; }

	[[nodiscard]] auto id() const { return static_cast<std::string>(sbs_descriptor->mIdentifier); }
	[[nodiscard]] auto label() const { return static_cast<std::string>(sbs_descriptor->mLabel); }
	[[nodiscard]] auto description() const { return static_cast<std::string>(sbs_descriptor->mGuiDescription); }
	[[nodiscard]] auto visible_condition() const { return static_cast<std::string>(sbs_descriptor->mGuiVisibleIf); }
	[[nodiscard]] auto is_color() const { return sbs_descriptor->mIsColor; }
	[[nodiscard]] auto is_floating_point() const { return sbs_descriptor->mIsFPFormat; }
	[[nodiscard]] auto usages() const { return sbs_descriptor->mChannelsStr | rn::to<std::vector<std::string>>; }
	[[nodiscard]] auto is_visible() const
	{
		if (instance) return sbs::VisibleIf::EvalVisibleIf(instance);
		return true;
	}
	[[nodiscard]] auto default_usage() const
	{
		return static_cast<std::string>(sbs_descriptor->defaultChannelUseStr());
	}


	auto load_from_file(const std::string& filename) -> void;
	auto load_from_buffer(void* data, int width, int height, PixelFormat format) -> void;
};

}