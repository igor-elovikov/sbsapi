#include "image.h"

#include <OpenImageIO/imageio.h>
namespace oiio = OIIO;

namespace sbsar {

auto get_oiio_type(const Image& image) -> oiio::TypeDesc {
	switch (image.format.precision) {

		case Precision::B8:
			return oiio::TypeUInt8;

		case Precision::B16:
			switch (image.format.dtype) {
				case DataType::INTEGER:
					return oiio::TypeUInt16;
				case DataType::FLOAT:
					return oiio::TypeHalf;
				default:
					return oiio::TypeDesc::NONE;
			}

		case Precision::B32:
			switch (image.format.dtype) {
				case DataType::INTEGER:
					return oiio::TypeUInt32;
				case DataType::FLOAT:
					return oiio::TypeFloat;
				default:
					return oiio::TypeDesc::NONE;
			}

		default:
			return oiio::TypeDesc::NONE;
	}
}

auto get_format_from_oiio(const oiio::TypeDesc& type_desc) -> PixelFormat {
	auto format = PixelFormat{};
	switch (type_desc.basetype) {
		case oiio::TypeDesc::UCHAR:
			format.dtype = DataType::INTEGER;
			format.precision = Precision::B8;
			break;

		case oiio::TypeDesc::HALF:
			format.dtype = DataType::FLOAT;
			format.precision = Precision::B16;
			break;

		case oiio::TypeDesc::UINT16:
			format.dtype = DataType::INTEGER;
			format.precision = Precision::B16;
			break;

		case oiio::TypeDesc::FLOAT:
			format.dtype = DataType::FLOAT;
			format.precision = Precision::B32;
			break;

		default:
			break;
	}

	return format;
}

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
	auto oiio_type = get_oiio_type(*this);

	auto image_specs = oiio::ImageSpec(width, height, format.num_channels, oiio_type);
	auto image_writer = oiio::ImageOutput::create(filename);

	image_writer->open(filename, image_specs);
	image_writer->write_image(oiio_type, get_raw_data());
	image_writer->close();

	spdlog::debug("Save image to: {}", filename);
}

auto Image::load(const std::string& filename) -> void {
	auto input = oiio::ImageInput::open(filename);
	if (!input) return;

	const auto& spec = input->spec();

	width = spec.width;
	height = spec.height;

	auto buffer_size = width * height * spec.nchannels * spec.channel_bytes();
	data.resize(buffer_size);

	format = get_format_from_oiio(spec.format);
	format.num_channels = spec.nchannels;

	switch (format.num_channels) {
		case 1:
			format.format = DataFormat::GRAYSCALE;
			break;
		case 3:
			format.format = DataFormat::RGB;
			break;
		case 4:
			format.format = DataFormat::RGBA; // TODO: handle RGBx
			break;
		default:
			break;
	}

	input->read_image(spec.format, (void*)data.data());

}

}