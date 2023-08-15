#pragma once

#include <type_traits>
#include <variant>
#include <stdexcept>

template <typename> inline constexpr bool always_false_v = false;

template <typename R, typename E>
class result {
	static constexpr auto is_pointer = std::is_pointer_v<R>;
	static constexpr auto is_reference = std::is_reference_v<R>;

	static constexpr auto get_storage_type = []() -> decltype(auto) {
		if constexpr (is_pointer) return R{nullptr};
		else if constexpr (is_reference) return std::add_pointer_t<R>{nullptr};
	};

	using storage_type = std::conditional_t<is_pointer || is_reference,
											decltype(result<R, E>::get_storage_type()),
											R>;

	using base_type = std::conditional_t<is_pointer,
										 std::remove_pointer_t<R>,
										 std::decay_t<R>>;

	std::variant<storage_type, E> value_;

	auto* get_ptr() const {
		if constexpr (is_pointer || is_reference) return std::get<storage_type>(value_);
		else return &std::get<storage_type>(value_);
	}
	auto& get_ref() const {
		if constexpr (is_pointer || is_reference) return *std::get<storage_type>(value_);
		else return std::get<storage_type>(value_);
	}

public:

	result() = delete;

	template <typename T = R>
	result(base_type& value, typename std::enable_if_t<!(std::is_pointer_v<T> || std::is_reference_v<T>)>* = 0)
	  : value_(value) { }

	template <typename T = R>
	result(base_type&& value, typename std::enable_if_t<!(std::is_pointer_v<T> || std::is_reference_v<T>)>* = 0)
	  : value_(value) { }

	template <typename T = R>
	result(base_type& value, typename std::enable_if_t<std::is_pointer_v<T> || std::is_reference_v<T>>* = 0)
	  : value_(&value) { }

	template <typename T = R>
	result(base_type&&, typename std::enable_if_t<std::is_pointer_v<T> || std::is_reference_v<T>>* = 0) {
		static_assert(always_false_v<R>, "Can't construct result type holding reference from rvalue!");
	}

	result(const R& r)
	  : value_(r) { }

	result(const E& error)
	  : value_(error) { }
	result(const E&& error)
	  : value_(error) { }

	auto& operator*() const { return get_ref(); }
	auto* operator->() const { return get_ptr(); }

	operator bool() const { return has_result(); }

	[[nodiscard]] auto has_result() const { return std::holds_alternative<storage_type>(value_); }
	[[nodiscard]] auto has_error() const { return std::holds_alternative<E>(value_); }

	[[nodiscard]] auto expect() -> R {
		if (has_result()) {
			if constexpr (is_pointer)
				return get_ptr();
			else
				return get_ref();
		}
		else
			throw std::runtime_error("Expected value is not present in result");
	}

	[[nodiscard]] auto& error() {
		return std::get<E>(value_);
	}

	template <typename F, typename ...Args>
	auto apply_if(F&& func, Args&& ...args) -> result& {
		if (has_result()) {
			if constexpr (std::is_invocable_v<F, base_type&, Args...>)
				std::invoke(func, get_ref(), std::forward<Args>(args)...);
			else if constexpr (std::is_invocable_v<F, base_type*, Args...>)
				std::invoke(func, get_ptr(), std::forward<Args>(args)...);
			else
				static_assert(always_false_v<F>, "Apply: Function is not invokable with result!");
		}
		return *this;
	}

	template <typename>
	struct ret_t : std::false_type { };

	template <typename Ret, typename Err>
	struct ret_t<result<Ret, Err>> : std::true_type {
	};

	template <typename F, typename ...Args>
	auto map(F&& func, Args&& ...args) {
		if constexpr (std::is_invocable_v<F, base_type&, Args...>) {

			using return_type = std::invoke_result_t<F, base_type&, Args...>;
			if constexpr (!ret_t<return_type>()) {
				static_assert(always_false_v<F>, "Map function should return result type");
			}
			else {
				using error_type = typename ret_t<return_type>::error_type;
				if (has_result())
					return std::invoke(func, get_ref(), std::forward<Args>(args)...);
				else if constexpr (std::is_convertible_v<E, error_type>)
					return return_type(error_type(error()));
				else
					static_assert(always_false_v<F>, "Error types are not convertible in map function.");
			}
		}
		else if constexpr (std::is_invocable_v<F, base_type*, Args...>)
			return std::invoke(func, get_ptr(), std::forward<Args>(args)...);
		else
			static_assert(always_false_v<F>, "Map: Function is not invokable with result!");
	}
};