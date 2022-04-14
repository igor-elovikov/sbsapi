#pragma once

#include "common.h"
#include "types.h"

#include <variant>
#include <boost/hana.hpp>

namespace hana = boost::hana;
using namespace hana::literals;

template <typename T, SubstanceIOType IOType>
constexpr auto sbs_types_info = hana::make_tuple(
  hana::type_c<T>, // builtin instance_type
  hana::integral_constant<SubstanceIOType, IOType>{} // IO enum
);

constexpr auto get_builtin_type = hana::reverse_partial(hana::at, 0_c);
constexpr auto get_type_io_c = hana::reverse_partial(hana::at, 1_c);
constexpr auto get_type_io(auto info) { return get_type_io_c(info).value; }

constexpr auto sbs_parm_types = hana::make_tuple(
  sbs_types_info<float, Substance_IOType_Float>,
  sbs_types_info<sbs::Vec2Float, Substance_IOType_Float2>,
  sbs_types_info<sbs::Vec3Float, Substance_IOType_Float3>,
  sbs_types_info<sbs::Vec4Float, Substance_IOType_Float4>,
  sbs_types_info<int, Substance_IOType_Integer>,
  sbs_types_info<sbs::Vec2Int, Substance_IOType_Integer2>,
  sbs_types_info<sbs::Vec3Int, Substance_IOType_Integer3>,
  sbs_types_info<sbs::Vec4Int, Substance_IOType_Integer4>,
  sbs_types_info<sbs::string, Substance_IOType_String>
);

template <typename T>
constexpr auto info_from_type = hana::find_if(sbs_parm_types, [](auto t) {
	return hana::equal(get_builtin_type(t), hana::type_c<T>);
}).value();

template <SubstanceIOType io>
constexpr auto info_from_io = hana::find_if(sbs_parm_types, [](auto t) {
	return hana::equal(get_type_io_c(t), hana::integral_constant<SubstanceIOType, io>{});
}).value();

template <typename ...Ts>
constexpr auto as_variant_helper(hana::tuple<Ts...>) -> std::variant<std::monostate, typename Ts::type...>;
template <auto tuple>
using as_variant_t = decltype(as_variant_helper(tuple));
constexpr auto builtin_types = hana::transform(sbs_parm_types, get_builtin_type);

using sbs_value_variant_t = as_variant_t<builtin_types>;

template <typename T>
struct SbsInput {
	using instance_type = sbs::InputInstanceNumerical<T>*;
	using descriptor_type = const sbs::InputDescNumerical<T>*;
	using choice_type = std::vector<std::pair<T, std::string>>;

	static auto get_value(instance_type instance) { return instance->getValue(); }
	static auto apply_value(instance_type instance, const T& value) { instance->setValue(value); }

	static auto get_default_value(descriptor_type desc) { return desc->mDefaultValue; }
	static auto get_max_value(descriptor_type desc) { return desc->mMinValue; }
	static auto get_min_value(descriptor_type desc) { return desc->mMinValue; }
	static auto get_slider_step(descriptor_type desc) { return desc->mSliderStep; }
	static auto get_slider_clamp(descriptor_type desc) { return desc->mSliderClamp; }
	static auto get_label_true(descriptor_type desc) { return desc->mLabelTrue; }
	static auto get_label_false(descriptor_type desc) { return desc->mLabelFalse; }
	static auto choices(descriptor_type desc) -> choice_type
	{
		return desc->mEnumValues | rn::to<choice_type>;
	}
};

template <>
struct SbsInput<sbs::string> {
	using instance_type = sbs::InputInstanceString*;
	using descriptor_type = const sbs::InputDescString*;
	using choice_type = std::vector<std::pair<sbs::string, std::string>>;

	static auto get_value(instance_type instance) { return instance->getString(); }
	static auto apply_value(instance_type instance, const sbs::string& value) -> void
	{
		instance->setString(value);
	}

	static auto get_default_value(descriptor_type desc) { return desc->mDefaultValue; }
	static auto get_max_value(descriptor_type desc) { return sbs::string{}; }
	static auto get_min_value(descriptor_type desc) { return sbs::string{}; }
	static auto get_slider_step(descriptor_type desc) { return 0.f; }
	static auto get_slider_clamp(descriptor_type desc) { return false; }
	static auto get_label_true(descriptor_type desc) { return std::string{}; }
	static auto get_label_false(descriptor_type desc) { return std::string{}; }

	static auto choices(descriptor_type desc) -> choice_type
	{
		return {};
	}
};
