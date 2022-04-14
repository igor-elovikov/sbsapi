#include "common.h"
#include "Input.h"
#include "Image.h"

namespace sbsar {

auto Input::load(const std::string& filename) -> void
{
	if (!instance) return;

	auto image = Image(filename);

	auto texture = SubstanceTexture{
		.buffer = image.get_raw_data(),
		.level0Width =  static_cast<unsigned short>(image.width),
		.level0Height = static_cast<unsigned short>(image.height),
	  	.pixelFormat = static_cast<unsigned char>(image.format.as_sbs_pixelformat()),
		.channelsOrder = Substance_ChanOrder_RGBA,
	};

	input_image = sbs::InputImage::create(texture);
	instance->setImage(input_image);
}


}