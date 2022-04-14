#pragma once

#include "common.h"

namespace sbsar {

enum class ParameterType {
	STRING,
	FLOAT,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	INTEGER,
	INTEGER2,
	INTEGER3,
	INTEGER4
};

enum class ParameterWidget {
	NONE,
	SLIDER,
	ANGLE,
	COLOR,
	TOGGLE,
	COMBOBOX,
	IMAGE,
	POSITION
};

// TODO: no channels order yet

enum class Precision {
	B8,
	B16,
	B32
};

enum class DataFormat {
	RGB,
	RGBA,
	RGBX,
	GRAYSCALE
};

enum class DataType {
	INTEGER,
	FLOAT
};

struct ImageFormat {
	Precision precision;
	DataFormat format;
	DataType dtype;
	int num_channels;

	ImageFormat() = default;
	explicit ImageFormat(SubstancePixelFormat pixel_format)
	{
		int sbs_precision = pixel_format & Substance_PF_MASK_RAWPrecision;
		int channels = pixel_format & Substance_PF_MASK_RAWChannels;

		switch (sbs_precision) {
			case Substance_PF_8I:
				dtype = DataType::INTEGER;
				precision = Precision::B8;
				break;

			case Substance_PF_16I:
				dtype = DataType::INTEGER;
				precision = Precision::B16;
				break;

			case Substance_PF_16F:
				dtype = DataType::FLOAT;
				precision = Precision::B16;
				break;

			case Substance_PF_32F:
				dtype = DataType::FLOAT;
				precision = Precision::B32;
				break;

			default:
				break;
		}

		switch (channels) {
			case Substance_PF_RGBA:
				format = DataFormat::RGBA;
				break;

			case Substance_PF_RGBx:
				format = DataFormat::RGBX;
				break;

			case Substance_PF_RGB:
				format = DataFormat::RGB;
				break;

			case Substance_PF_L:
				format = DataFormat::GRAYSCALE;
				break;

			default:
				break;
		}

		num_channels = 0;
		if (format == DataFormat::RGBA || format == DataFormat::RGBX) num_channels = 4;
		if (format == DataFormat::GRAYSCALE) num_channels = 1;
		if (format == DataFormat::RGB) num_channels = 3;
	}

	[[nodiscard]] auto as_sbs_pixelformat() const -> SubstancePixelFormat
	{
		auto result = 0u;
		switch (dtype) {
			case DataType::INTEGER:
				switch (precision) {
					case Precision::B8:
						result |= Substance_PF_8I;
						break;
					case Precision::B16:
						result |= Substance_PF_16I;
						break;
					case Precision::B32:
						break;
				}
			case DataType::FLOAT:
				switch (precision) {
					case Precision::B8:
						break;
					case Precision::B16:
						result |= Substance_PF_16F;
						break;
					case Precision::B32:
						result |= Substance_PF_32F;
						break;
				}
		}

		switch (format) {
			case DataFormat::RGB:
				result |= Substance_PF_RGB;
				break;
			case DataFormat::RGBA:
				result |= Substance_PF_RGBA;
				break;
			case DataFormat::RGBX:
				result |= Substance_PF_RGBx;
				break;
			case DataFormat::GRAYSCALE:
				result |= Substance_PF_L;
				break;
		}

		return (SubstancePixelFormat)result;
	}
};

enum class Resolution {
	x1,
	x2,
	x4,
	x16,
	x32,
	x64,
	x128,
	x256,
	x512,
	x1024,
	x2048,
	x4096,
	x8192
};

static inline const auto io_types_map = std::unordered_map<SubstanceIOType, ParameterType>{
  {Substance_IOType_String, ParameterType::STRING},
  {Substance_IOType_Float, ParameterType::FLOAT},
  {Substance_IOType_Float2, ParameterType::FLOAT2},
  {Substance_IOType_Float3, ParameterType::FLOAT3},
  {Substance_IOType_Float4, ParameterType::FLOAT4},
  {Substance_IOType_Integer, ParameterType::INTEGER},
  {Substance_IOType_Integer2, ParameterType::INTEGER2},
  {Substance_IOType_Integer3, ParameterType::INTEGER3},
  {Substance_IOType_Integer4, ParameterType::INTEGER4}
};

static inline auto get_image_format(SubstancePixelFormat pixel_format) { return ImageFormat(pixel_format); }

}
