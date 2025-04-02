#include "image.h"

namespace sbsar {

Image::Image(sbs::OutputInstance* output_instance)
  : format() {
	render_result = output_instance->grabResult();

	if (!render_result) {
		spdlog::warn("Can't grab results from output [{}]", output_instance->mDesc.mIdentifier);
		return;
	}

	if (!render_result->isImage()) {
		spdlog::warn("Output [{}] is not an image and it can't be saved as one", output_instance->mDesc.mIdentifier);
		return;
	}

	rendered_image = dynamic_cast<sbs::RenderResultImage*>(render_result.get());
	if (!rendered_image) {
		spdlog::warn("Output [{}] couldn't get result image pointer", output_instance->mDesc.mIdentifier);
		return;
	}

	is_rendered = true;
	auto texture = rendered_image->getTexture();

	format = PixelFormat((SubstancePixelFormat)texture.pixelFormat);
	width = texture.level0Width;
	height = texture.level0Height;

	spdlog::debug("dtype: {}", (int)format.dtype);

	spdlog::debug("Grab image from output [{}]: Size:{}x{} Format: {:07b}",
	  output_instance->mDesc.mIdentifier, width, height, texture.pixelFormat);
}

auto Image::save(const std::string& filename) const -> void {

}

auto Image::load(const std::string& filename) -> void {


}

}