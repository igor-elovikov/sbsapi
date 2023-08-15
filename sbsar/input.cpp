#include "common.h"
#include "input.h"
#include "image.h"

namespace sbsar {

auto Input::load_from_file(const std::string& filename) -> void {
	if (!instance) return;

	auto image = Image(filename);

	auto texture = SubstanceTexture{};
	texture.buffer = image.get_raw_data();
	texture.level0Width = static_cast<unsigned short>(image.width);
	texture.level0Height = static_cast<unsigned short>(image.height);
	texture.pixelFormat = static_cast<unsigned char>(image.format.as_sbs_pixelformat());
	texture.channelsOrder = Substance_ChanOrder_RGBA;

	input_image = sbs::InputImage::create(texture);
	instance->setImage(input_image);
}

auto Input::load_from_buffer(void* data, int width, int height, PixelFormat format) -> void {
	if (!instance) return;

	auto texture = SubstanceTexture{};
	texture.buffer = data;
	texture.level0Width = static_cast<unsigned short>(width);
	texture.level0Height = static_cast<unsigned short>(height);
	texture.pixelFormat = static_cast<unsigned char>(format.as_sbs_pixelformat());
	texture.channelsOrder = Substance_ChanOrder_RGBA;

	input_image = sbs::InputImage::create(texture);
	instance->setImage(input_image);
}

}