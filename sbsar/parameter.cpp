#include "parameter.h"
#include "context.h"

namespace sbsar {

auto Parameter::slider_step() const -> std::optional<float> {
	switch (type()) {
		case ParameterType::STRING:
			return std::nullopt;
		case ParameterType::FLOAT:
			return slider_step_as<float>();
		case ParameterType::FLOAT2:
			return slider_step_as<sbs::Vec2Float>();
		case ParameterType::FLOAT3:
			return slider_step_as<sbs::Vec3Float>();
		case ParameterType::FLOAT4:
			return slider_step_as<sbs::Vec4Float>();
		case ParameterType::INTEGER:
			return slider_step_as<int>();
		case ParameterType::INTEGER2:
			return slider_step_as<sbs::Vec2Int>();
		case ParameterType::INTEGER3:
			return slider_step_as<sbs::Vec3Int>();
		case ParameterType::INTEGER4:
			return slider_step_as<sbs::Vec4Int>();
	}
	return std::nullopt;
}

auto Parameter::slider_clamp() const -> std::optional<bool> {
	switch (type()) {
		case ParameterType::STRING:
			return std::nullopt;
		case ParameterType::FLOAT:
			return slider_clamp_as<float>();
		case ParameterType::FLOAT2:
			return slider_clamp_as<sbs::Vec2Float>();
		case ParameterType::FLOAT3:
			return slider_clamp_as<sbs::Vec3Float>();
		case ParameterType::FLOAT4:
			return slider_clamp_as<sbs::Vec4Float>();
		case ParameterType::INTEGER:
			return slider_clamp_as<int>();
		case ParameterType::INTEGER2:
			return slider_clamp_as<sbs::Vec2Int>();
		case ParameterType::INTEGER3:
			return slider_clamp_as<sbs::Vec3Int>();
		case ParameterType::INTEGER4:
			return slider_clamp_as<sbs::Vec4Int>();
	}
	return std::nullopt;
}

auto Parameter::default_value() const -> Value {
	auto result = Value{};
	switch (type()) {
		case ParameterType::STRING:
			if (auto value = default_value_as<sbs::string>()) {
				result = std::string(*value);
			}
			break;

		case ParameterType::FLOAT:
			if (auto value = default_value_as<float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT2:
			if (auto value = default_value_as<sbs::Vec2Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT3:
			if (auto value = default_value_as<sbs::Vec3Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT4:
			if (auto value = default_value_as<sbs::Vec4Float>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER:
			if (auto value = default_value_as<int>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER2:
			if (auto value = default_value_as<sbs::Vec2Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER3:
			if (auto value = default_value_as<sbs::Vec3Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER4:
			if (auto value = default_value_as<sbs::Vec4Int>()) {
				result = *value;
			}
			break;
	}
	return result;
}

auto Parameter::label_true() const -> std::optional<std::string> {
	auto desc_pointer = descriptor_for<int>();
	if (!desc_pointer || widget() != ParameterWidget::TOGGLE) return std::nullopt;
	return std::string(desc_pointer->mLabelTrue);
}

auto Parameter::label_false() const -> std::optional<std::string> {
	auto desc_pointer = descriptor_for<int>();
	if (!desc_pointer || widget() != ParameterWidget::TOGGLE) return std::nullopt;
	return std::string(desc_pointer->mLabelFalse);
}

auto Parameter::max_value() const -> Value {
	auto result = Value{};
	switch (type()) {
		case ParameterType::STRING:
			if (auto value = max_value_as<sbs::string>()) {
				result = std::string(*value);
			}
			break;

		case ParameterType::FLOAT:
			if (auto value = max_value_as<float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT2:
			if (auto value = max_value_as<sbs::Vec2Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT3:
			if (auto value = max_value_as<sbs::Vec3Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT4:
			if (auto value = max_value_as<sbs::Vec4Float>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER:
			if (auto value = max_value_as<int>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER2:
			if (auto value = max_value_as<sbs::Vec2Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER3:
			if (auto value = max_value_as<sbs::Vec3Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER4:
			if (auto value = max_value_as<sbs::Vec4Int>()) {
				result = *value;
			}
			break;
	}
	return result;
}

auto Parameter::min_value() const -> Value {
	auto result = Value{};
	switch (type()) {
		case ParameterType::STRING:
			if (auto value = min_value_as<sbs::string>()) {
				result = std::string(*value);
			}
			break;

		case ParameterType::FLOAT:
			if (auto value = min_value_as<float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT2:
			if (auto value = min_value_as<sbs::Vec2Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT3:
			if (auto value = min_value_as<sbs::Vec3Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT4:
			if (auto value = min_value_as<sbs::Vec4Float>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER:
			if (auto value = min_value_as<int>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER2:
			if (auto value = min_value_as<sbs::Vec2Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER3:
			if (auto value = min_value_as<sbs::Vec3Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER4:
			if (auto value = min_value_as<sbs::Vec4Int>()) {
				result = *value;
			}
			break;
	}
	return result;
}

auto Parameter::choices() const -> std::optional<Choices> {
	if (type() != ParameterType::INTEGER) return std::nullopt;
	return choices_as<int>();
}

auto Parameter::get() const -> Value {
	auto result = Value{};
	switch (type()) {
		case ParameterType::STRING:
			if (auto value = get_as<sbs::string>()) {
				result = std::string(*value);
			}
			break;

		case ParameterType::FLOAT:
			if (auto value = get_as<float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT2:
			if (auto value = get_as<sbs::Vec2Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT3:
			if (auto value = get_as<sbs::Vec3Float>()) {
				result = *value;
			}
			break;

		case ParameterType::FLOAT4:
			if (auto value = get_as<sbs::Vec4Float>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER:
			if (auto value = get_as<int>()) {
				result = *value;
			}
			break;

		case ParameterType::INTEGER2:
			if (auto value = get_as<sbs::Vec2Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER3:
			if (auto value = get_as<sbs::Vec3Int>()) {
				result = *value;
			}
			break;
		case ParameterType::INTEGER4:
			if (auto value = get_as<sbs::Vec4Int>()) {
				result = *value;
			}
			break;
	}
	return result;
}

auto Parameter::log_warn(const std::string& message) -> void {
	ctx->log_warn(message);
}

auto Parameter::set(const Value& v) -> void {

	switch (type()) {

		case ParameterType::STRING:
			set_from_variant<std::string>(v);
			break;
		case ParameterType::FLOAT:
			set_from_variant<float>(v);
			break;
		case ParameterType::FLOAT2:
			set_from_variant<sbs::Vec2Float>(v);
			break;
		case ParameterType::FLOAT3:
			set_from_variant<sbs::Vec3Float>(v);
			break;
		case ParameterType::FLOAT4:
			set_from_variant<sbs::Vec4Float>(v);
			break;
		case ParameterType::INTEGER:
			set_from_variant<int>(v);
			break;
		case ParameterType::INTEGER2:
			set_from_variant<sbs::Vec2Int>(v);
			break;
		case ParameterType::INTEGER3:
			set_from_variant<sbs::Vec3Int>(v);
			break;
		case ParameterType::INTEGER4:
			set_from_variant<sbs::Vec4Int>(v);
			break;
	}
}

}

