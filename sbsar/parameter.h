#pragma once

#include "../utils/common.h"
#include "metatypes.h"
#include <substance/framework/visibleif.h>
#include <optional>

namespace sbsar {

using Value = meta::sbs_value_variant_t;
using Choice = std::pair<int, std::string>;
using Choices = std::vector<Choice>;

class Context;

class Parameter {

	friend class Graph;
	Context* ctx = nullptr;

	const sbs::InputDescBase* sbs_descriptor = nullptr;
	sbs::InputInstanceBase* instance = nullptr;

	template <typename T>
	[[nodiscard]] auto instance_for() const -> typename meta::SbsInput<T>::instance_type {
		return dynamic_cast<typename meta::SbsInput<T>::instance_type>(instance);
	}

	[[nodiscard]]
	auto numerical_desc() const {
		return sbs_descriptor->isNumerical()
			   ? dynamic_cast<const sbs::InputDescNumericalBase*>(sbs_descriptor)
			   : nullptr;
	}

	template <typename T>
	[[nodiscard]] auto slider_step_as() const -> std::optional<float> {
		meta::assert_sbs_type<T>();
		auto desc_pointer = descriptor_for<T>();
		if (!desc_pointer) return std::nullopt;
		if constexpr (meta::is_string<T>) return std::nullopt;
		else
			return desc_pointer->mSliderStep;
	}

	template <typename T>
	[[nodiscard]] auto slider_clamp_as() const -> std::optional<bool> {
		meta::assert_sbs_type<T>();
		auto desc_pointer = descriptor_for<T>();
		if (!desc_pointer) return std::nullopt;
		if constexpr (meta::is_string<T>) return std::nullopt;
		else
			return desc_pointer->mSliderClamp;
	}

	auto log_warn(const std::string& message) -> void;

	template <typename T>
	auto set_from_variant(const Value& v) -> void {
		if (std::holds_alternative<T>(v)) {
			set<T>(std::get<T>(v));
		}
		else {
			log_warn(fmt::format("Wrong type with setting parameter [{}]",
			  sbs_descriptor->mIdentifier));
		}
	}

public:
	Parameter() = default;
	Parameter(const Parameter&) = delete;
	Parameter(Parameter&&) = default;
	explicit Parameter(const sbs::InputDescBase* desc) { sbs_descriptor = desc; }

	[[nodiscard]] auto descriptor() const { return sbs_descriptor; }
	template <typename T>
	[[nodiscard]] auto descriptor_for() const -> typename meta::SbsInput<T>::descriptor_type {
		return dynamic_cast<typename meta::SbsInput<T>::descriptor_type>(sbs_descriptor);
	}

	[[nodiscard]] auto type() const { return io_types_map.at(sbs_descriptor->mType); }
	[[nodiscard]] auto id() const { return static_cast<std::string>(sbs_descriptor->mIdentifier); }
	[[nodiscard]] auto label() const { return static_cast<std::string>(sbs_descriptor->mLabel); }
	[[nodiscard]] auto group() const { return static_cast<std::string>(sbs_descriptor->mGuiGroup); }
	[[nodiscard]] auto usages() const { return 0 /*sbs_descriptor->mChannelsStr | rn::to<std::vector<std::string>>*/; }
	[[nodiscard]] auto description() const { return static_cast<std::string>(sbs_descriptor->mGuiDescription); }
	[[nodiscard]] auto visible_condition() const { return static_cast<std::string>(sbs_descriptor->mGuiVisibleIf); }
	[[nodiscard]] auto widget() const { return static_cast<ParameterWidget>(sbs_descriptor->mGuiWidget); }
	[[nodiscard]] auto user_tag() const { return static_cast<std::string>(sbs_descriptor->mUserTag); }

	template <typename T>
	[[nodiscard]] auto is() const { return meta::get_type_io<T>() == sbs_descriptor->mType; }

	template <typename T>
	[[nodiscard]] auto min_value_as() const -> std::optional<T> {
		auto desc_pointer = descriptor_for<T>();
		if (!desc_pointer) return std::nullopt;

		if constexpr (meta::is_numeric<T>)
			return desc_pointer->mMinValue;

		return std::nullopt;
	}

	template <typename T>
	[[nodiscard]] auto max_value_as() const -> std::optional<T> {
		auto desc_pointer = descriptor_for<T>();
		if (!desc_pointer) return std::nullopt;

		if constexpr (meta::is_numeric<T>)
			return desc_pointer->mMaxValue;

		return std::nullopt;
	}

	template <typename T>
	[[nodiscard]] auto default_value_as() const -> std::optional<T> {
		auto desc_pointer = descriptor_for<T>();
		if (!desc_pointer) return std::nullopt;

		if constexpr (meta::is_numeric<T>)
			return desc_pointer->mDefaultValue;

		return std::nullopt;
	}

	[[nodiscard]] auto component_labels() const {
		if (auto descriptor = numerical_desc())
			return std::vector<std::string>{};
		else
			return std::vector{label()};
	}

	template <typename T>
	[[nodiscard]] auto choices_as() const -> std::optional<Choices> {
		if constexpr (std::is_same_v<T, int>) {
			if (auto desc = descriptor_for<int>())
				return desc->mEnumValues | rn::to<Choices>;
		}
		return std::nullopt;
	}

	[[nodiscard]] auto is_visible() const {
		if (instance) return sbs::VisibleIf::EvalVisibleIf(instance);
		return true;
	}

	template <typename T>
	[[nodiscard]] auto get_as() const -> std::optional<T> {
		if (instance) {
			auto instance_pointer = instance_for<T>();
			if (!instance_pointer) return std::nullopt;
			if constexpr (meta::is_numeric<T>)
				return instance_pointer->getValue();
			else if constexpr (meta::is_string<T>)
				return static_cast<T>(instance_pointer->getString());
		}
		else {
			return default_value_as<T>();
		}
	}

	[[nodiscard]] auto slider_step() const -> std::optional<float>;
	[[nodiscard]] auto slider_clamp() const -> std::optional<bool>;
	[[nodiscard]] auto label_true() const -> std::optional<std::string>;
	[[nodiscard]] auto label_false() const -> std::optional<std::string>;
	[[nodiscard]] auto default_value() const -> Value;
	[[nodiscard]] auto max_value() const -> Value;
	[[nodiscard]] auto min_value() const -> Value;
	[[nodiscard]] auto choices() const -> std::optional<Choices>;

	[[nodiscard]] auto get() const -> Value;

	template <typename T>
	auto set(const T& v) -> void {
		if (!instance) return;
		auto instance_pointer = instance_for<T>();
		if (!instance_pointer) {
			log_warn(fmt::format("Wrong type with setting parameter [{}] with value: {}",
			  sbs_descriptor->mIdentifier, v));
			return;
		}

		if constexpr (meta::is_numeric<T>)
			instance_pointer->setValue(v);
		else if constexpr (meta::is_string<T>)
			instance_pointer->setString(static_cast<sbs::string>(v));
	}

	auto set(const Value& v) -> void;
	auto set(const char* v) -> void { set(sbs::string(v)); }
	//auto set(const std::string& v) -> void { set(sbs::string(v)); }
};

}
