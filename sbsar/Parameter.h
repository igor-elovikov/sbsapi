#pragma once

#include "common.h"
#include "metatypes.h"
#include <substance/framework/visibleif.h>

namespace sbsar {

class Parameter {

	friend class Graph;

	const sbs::InputDescBase* sbs_descriptor = nullptr;
	sbs::InputInstanceBase* instance = nullptr;

	template <typename T>
	[[nodiscard]] auto get_instance_pointer() const -> typename SbsInput<T>::instance_type
	{
		return dynamic_cast<typename SbsInput<T>::instance_type>(instance);
	}

	template <typename T>
	[[nodiscard]] auto get_desc_pointer() const -> typename SbsInput<T>::descriptor_type
	{
		return dynamic_cast<typename SbsInput<T>::descriptor_type>(sbs_descriptor);
	}

public:
	Parameter() = default;
	auto descriptor() { return sbs_descriptor; }

	[[nodiscard]] auto type() const { return io_types_map.at(sbs_descriptor->mType); }
	[[nodiscard]] auto label() const { return static_cast<std::string>(sbs_descriptor->mLabel); }
	[[nodiscard]] auto group() const { return static_cast<std::string>(sbs_descriptor->mGuiGroup); }
	[[nodiscard]] auto usages() const { return sbs_descriptor->mChannelsStr | rn::to<std::vector<std::string>>; }
	[[nodiscard]] auto description() const { return static_cast<std::string>(sbs_descriptor->mGuiDescription); }
	[[nodiscard]] auto visible_condition() const { return static_cast<std::string>(sbs_descriptor->mGuiVisibleIf); }
	[[nodiscard]] auto widget() const { return static_cast<ParameterWidget>(sbs_descriptor->mGuiWidget); }
	[[nodiscard]] auto user_tag() const { return static_cast<std::string>(sbs_descriptor->mUserTag); }
	[[nodiscard]] auto component_labels() const
	{
		if (sbs_descriptor->isNumerical()) {
			auto descriptor = dynamic_cast<const sbs::InputDescNumericalBase*>(sbs_descriptor);
			return descriptor->mGuiVecLabels | rn::to<std::vector<std::string>>;
		}
		else {
			return std::vector{label()};
		}
	}
	[[nodiscard]] auto is_visible() const
	{
		if (instance) return sbs::VisibleIf::EvalVisibleIf(instance);
		return true;
	}

	[[nodiscard]] auto slider_step() const -> float
	{
		auto result = 0.f;
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				auto desc_pointer = get_desc_pointer<builtin_t>();
				result = SbsInput<builtin_t>::get_slider_step(desc_pointer);
			}
		});
		return result;
	}

	[[nodiscard]] auto slider_clamp() const -> bool
	{
		auto result = false;
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				auto desc_pointer = get_desc_pointer<builtin_t>();
				result = SbsInput<builtin_t>::get_slider_clamp(desc_pointer);
			}
		});
		return result;
	}

	[[nodiscard]] auto label_true() const -> std::string
	{
		auto result = std::string {};
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				auto desc_pointer = get_desc_pointer<builtin_t>();
				result = SbsInput<builtin_t>::get_label_true(desc_pointer);
			}
		});
		return result;
	}

	[[nodiscard]] auto label_false() const -> std::string
	{
		auto result = std::string {};
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				auto desc_pointer = get_desc_pointer<builtin_t>();
				result = SbsInput<builtin_t>::get_label_false(desc_pointer);
			}
		});
		return result;
	}

	template <typename T>
	auto is() -> bool
	{
		static constexpr auto type_info = info_from_type<T>;
		static constexpr auto io = get_type_io(type_info);
		return io == sbs_descriptor->mType;
	}

	template <typename T>
	auto default_value_as() -> T
	{
		auto desc_pointer = get_desc_pointer<T>();
		if (!desc_pointer) return T{};

		return SbsInput<T>::get_default_value(desc_pointer);
	};

	auto default_value() -> sbs_value_variant_t
	{
		auto result = sbs_value_variant_t{};
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				result = default_value_as<builtin_t>();
			}
		});
		return result;
	};

	template <typename T>
	auto max_value_as() -> T
	{
		auto desc_pointer = get_desc_pointer<T>();
		if (!desc_pointer) return T{};

		return SbsInput<T>::get_max_value(desc_pointer);
	};

	auto max_value() -> sbs_value_variant_t
	{
		auto result = sbs_value_variant_t{};
		if (sbs_descriptor->isNumerical()) {
			hana::for_each(sbs_parm_types, [&](auto& t) {
				auto io = get_type_io(t);
				if (io == sbs_descriptor->mType) {
					auto builtin_type = get_builtin_type(t);
					using builtin_t = typename decltype(builtin_type)::type;
					result = max_value_as<builtin_t>();
				}
			});
		}
		return result;
	};

	template <typename T>
	auto min_value_as() -> T
	{
		auto desc_pointer = get_desc_pointer<T>();
		if (!desc_pointer) return T{};

		return SbsInput<T>::get_min_value(desc_pointer);
	};

	auto min_value() -> sbs_value_variant_t
	{
		auto result = sbs_value_variant_t{};
		if (sbs_descriptor->isNumerical()) {
			hana::for_each(sbs_parm_types, [&](auto& t) {
				auto io = get_type_io(t);
				if (io == sbs_descriptor->mType) {
					auto builtin_type = get_builtin_type(t);
					using builtin_t = typename decltype(builtin_type)::type;
					result = min_value_as<builtin_t>();
				}
			});
		}
		return result;
	};

	template <typename T>
	auto choices_as() -> typename SbsInput<T>::choice_type
	{
		auto desc_pointer = get_desc_pointer<T>();
		return SbsInput<T>::choices(desc_pointer);
	};

	auto choices() -> std::vector<std::pair<sbs_value_variant_t, std::string>>
	{
		using variant_choices = std::vector<std::pair<sbs_value_variant_t, std::string>>;
		auto result = std::vector<std::pair<sbs_value_variant_t, std::string>>{};
		if (sbs_descriptor->isNumerical()) {
			hana::for_each(sbs_parm_types, [&](auto& t) {
				auto io = get_type_io(t);
				if (io == sbs_descriptor->mType) {
					auto builtin_type = get_builtin_type(t);
					using builtin_t = typename decltype(builtin_type)::type;
					auto choices = choices_as<builtin_t>();
					for (auto&[v, k] : choices) {
						result.push_back(std::make_pair(sbs_value_variant_t(v), k));
					}
				}
			});
		}
		return result;
	};

	template <typename T>
	auto get_as() -> T
	{
		if (instance) {
			auto instance_pointer = get_instance_pointer<T>();
			if (!instance_pointer) return T{};
			return SbsInput<T>::get_value(instance_pointer);
		}
		else {
			return default_value_as<T>();
		}
	}

	auto get() -> sbs_value_variant_t
	{
		auto result = sbs_value_variant_t{};
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto io = get_type_io(t);
			if (io == sbs_descriptor->mType) {
				auto builtin_type = get_builtin_type(t);
				using builtin_t = typename decltype(builtin_type)::type;
				result = get_as<builtin_t>();
			}
		});
		return result;
	}

	template <typename T>
	auto set(const T& v) -> void
	{
		if (!instance) return;

		auto instance_pointer = get_instance_pointer<T>();
		if (!instance_pointer) return;

		SbsInput<T>::apply_value(instance_pointer, v);
	}

	template <>
	auto set(const sbs_value_variant_t& v) -> void
	{
		hana::for_each(sbs_parm_types, [&](auto& t) {
			auto builtin_type = get_builtin_type(t);
			using builtin_t = typename decltype(builtin_type)::type;
			if (std::holds_alternative<builtin_t>(v)) {
				set<builtin_t>(std::get<builtin_t>(v));
			}
		});
	}

	auto set(const char* v) -> void { set(sbs::string(v)); }

	template <>
	auto set(const std::string& v) -> void { set(sbs::string(v)); }

};

}
