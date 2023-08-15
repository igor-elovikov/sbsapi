#pragma once

#include "common.h"
#include "types.h"

#include <variant>

namespace sbsar::meta {

using sbs_value_variant_t = std::variant<
  std::monostate,
  float,
  sbs::Vec2Float,
  sbs::Vec3Float,
  sbs::Vec4Float,
  int,
  sbs::Vec2Int,
  sbs::Vec3Int,
  sbs::Vec4Int,
  std::string
>;

template <typename T, typename ...Ts>
static constexpr auto is_same_any() { return (std::is_same_v<T, Ts> || ...); }

template <typename T>
static constexpr auto is_supported = is_same_any
  <T, float, sbs::Vec2Float, sbs::Vec3Float ,sbs::Vec4Float ,
   int, sbs::Vec2Int, sbs::Vec3Int, sbs::Vec4Int, std::string, sbs::string>();

template <typename T>
static constexpr auto is_numeric = is_same_any
  <T, float, sbs::Vec2Float, sbs::Vec3Float ,sbs::Vec4Float ,
  int, sbs::Vec2Int, sbs::Vec3Int, sbs::Vec4Int>();

template <typename T>
static constexpr auto is_float = is_same_any
  <T, float, sbs::Vec2Float, sbs::Vec3Float ,sbs::Vec4Float>();

template <typename T>
static constexpr auto is_int = is_same_any
  <T, int, sbs::Vec2Int, sbs::Vec3Int, sbs::Vec4Int>();

template <typename T>
static constexpr auto is_string = is_same_any
  <T, sbs::string, std::string>();

template<typename T>
static constexpr auto assert_sbs_type() {
	static_assert(is_supported<T>, "Type is not supported for Substance Parameters");
}

template <typename T>
struct SbsInput {
	using instance_type = sbs::InputInstanceNumerical<T>*;
	using descriptor_type = const sbs::InputDescNumerical<T>*;
};

template <>
struct SbsInput<sbs::string> {
	using instance_type = sbs::InputInstanceString*;
	using descriptor_type = const sbs::InputDescString*;
};

template <>
struct SbsInput<std::string> {
	using instance_type = sbs::InputInstanceString*;
	using descriptor_type = const sbs::InputDescString*;
};

template <typename T>
static constexpr auto get_type_io() {
	if constexpr (std::is_same_v<T, float>) return Substance_IOType_Float;
	else if constexpr (std::is_same_v<T, sbs::Vec2Float>) return Substance_IOType_Float2;
	else if constexpr (std::is_same_v<T, sbs::Vec3Float>) return Substance_IOType_Float3;
	else if constexpr (std::is_same_v<T, sbs::Vec4Float>) return Substance_IOType_Float4;
	else if constexpr (std::is_same_v<T, int>) return Substance_IOType_Integer;
	else if constexpr (std::is_same_v<T, sbs::Vec2Int>) return Substance_IOType_Integer2;
	else if constexpr (std::is_same_v<T, sbs::Vec3Int>) return Substance_IOType_Integer3;
	else if constexpr (std::is_same_v<T, sbs::Vec4Int>) return Substance_IOType_Integer4;
	else if constexpr (std::is_same_v<T, sbs::string>) return Substance_IOType_String;
	else assert_sbs_type<T>();
}

}
