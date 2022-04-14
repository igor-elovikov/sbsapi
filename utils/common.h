//
// Created by elovikov on 30/03/2022.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <substance/framework/framework.h>

#include <spdlog/spdlog.h>
#include <range/v3/all.hpp>

#include "sbsar/types.h"

namespace rn = ranges;
namespace vi = rn::views;
namespace sbs = SubstanceAir;

template <class Container>
auto load_file(Container& container, const std::string& path, bool binary)
{
	std::ifstream is(
	  path.c_str(),
	  std::ios::in | (binary ? std::ios::binary : (std::ios::openmode)0));
	is.seekg(0, std::ios::end);
	container.resize(is.tellg());
	is.seekg(0, std::ios::beg);
	is.read((char*)&container[0], container.size());
}

template <>
struct fmt::formatter<sbs::Vec2Float> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec2Float v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec2Float [{}, {}]", v.x, v.y);
	}
};

template <>
struct fmt::formatter<sbs::Vec3Float> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec3Float v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec3Float [{}, {}, {}]", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<sbs::Vec4Float> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec4Float v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec4Float [{}, {}, {}, {}]", v.x, v.y, v.z, v.w);
	}
};

template <>
struct fmt::formatter<sbs::Vec2Int> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec2Int v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec2Int [{}, {}]", v.x, v.y);
	}
};

template <>
struct fmt::formatter<sbs::Vec3Int> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec3Int v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec3Int [{}, {}, {}]", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<sbs::Vec4Int> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbs::Vec4Int v, FormatContext& ctx)
	{
		return fmt::format_to(ctx.out(), "Vec4Int [{}, {}, {}, {}]", v.x, v.y, v.z, v.w);
	}
};

template <>
struct fmt::formatter<sbsar::Precision> : fmt::formatter<std::string> {
	template <typename FormatContext>
	auto format(sbsar::Precision v, FormatContext& ctx)
	{
		switch (v) {
			case sbsar::Precision::B8:
				return fmt::format_to(ctx.out(), "8 Bits");
			case sbsar::Precision::B16:
				return fmt::format_to(ctx.out(), "16 Bits");
			case sbsar::Precision::B32:
				return fmt::format_to(ctx.out(), "32 Bits");
		}
	}
};
