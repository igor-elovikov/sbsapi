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


auto Output::override_format(const OutputResolution& resolution) -> void
{
	if (!instance) return;

	auto sbs_output_format = sbs::OutputFormat{};
	if (resolution.width != OutputSize::NONE && resolution.height != OutputSize::NONE) {

		sbs_output_format.forceWidth = static_cast<unsigned int>(resolution.width);
		sbs_output_format.forceHeight = static_cast<unsigned int>(resolution.height);
	}

	instance->overrideFormat(sbs_output_format);
}

auto Output::override_format(const PixelFormat& pixel_format) -> void
{
	if (!instance) return;

	auto sbs_output_format = sbs::OutputFormat{};
	sbs_output_format.format = pixel_format.as_sbs_pixelformat();

	instance->overrideFormat(sbs_output_format);

}
auto Output::override_format(const OutputFormatOverride& format_override) -> void
{
	if (!instance) return;

	auto sbs_output_format = sbs::OutputFormat{};
	sbs_output_format.format = format_override.format.as_sbs_pixelformat();
	if (format_override.resolution.width != OutputSize::NONE
	  && format_override.resolution.height != OutputSize::NONE) {

		sbs_output_format.forceWidth = static_cast<unsigned int>(format_override.resolution.width);
		sbs_output_format.forceHeight = static_cast<unsigned int>(format_override.resolution.height);
	}

	instance->overrideFormat(sbs_output_format);
}


}