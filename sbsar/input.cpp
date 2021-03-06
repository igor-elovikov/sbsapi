#include "common.h"
#include "input.h"
#include "image.h"

namespace sbsar {

auto Input::load_from_file(const std::string& filename) -> void
{
	if (!instance) return;

	auto image = Image(filename);

	auto texture = SubstanceTexture{
	  .buffer = image.get_raw_data(),
	  .level0Width = static_cast<unsigned short>(image.width),
	  .level0Height = static_cast<unsigned short>(image.height),
	  .pixelFormat = static_cast<unsigned char>(image.format.as_sbs_pixelformat()),
	  .channelsOrder = Substance_ChanOrder_RGBA,
	};

	input_image = sbs::InputImage::create(texture);
	instance->setImage(input_image);
}

auto Input::load_from_buffer(void* data, int width, int height, PixelFormat format) -> void
{
	if (!instance) return;

	auto texture = SubstanceTexture{
	  .buffer = data,
	  .level0Width = static_cast<unsigned short>(width),
	  .level0Height = static_cast<unsigned short>(height),
	  .pixelFormat = static_cast<unsigned char>(format.as_sbs_pixelformat()),
	  .channelsOrder = Substance_ChanOrder_RGBA
	};

	input_image = sbs::InputImage::create(texture);
	instance->setImage(input_image);
}

}