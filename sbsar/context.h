#pragma once

#include "../utils/common.h"
#include "package.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/base_sink.h"

namespace sbsar {

using LogCallback = auto (*)(const char* message, uint32_t size) -> void;

template <typename Mutex>
class LogSink : public spdlog::sinks::base_sink<Mutex> {

	LogCallback callback = nullptr;

public:

	explicit LogSink(LogCallback log_callback)
	  : callback(log_callback) { }

protected:

	void sink_it_(const spdlog::details::log_msg& msg) override {
		if (!callback) return;
		auto formatted = spdlog::memory_buf_t{};
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		auto message = fmt::to_string(formatted);
		callback(message.c_str(), message.size());
	}

	void flush_() override { }
};

using LogSinkMt = LogSink<std::mutex>;

class Context {

	auto init_logging() -> void
	{
		logger = spdlog::stdout_color_mt("sbsar");
		logger->set_level(spdlog::level::debug);
	}

	std::unique_ptr<sbs::Renderer> renderer;
	std::unordered_map<std::string, Package> packages;

	std::shared_ptr<spdlog::logger> logger;

public:

	explicit Context(bool enable_renderer = true);

	explicit Context(LogCallback callback, bool enable_renderer = true);

	explicit Context(size_t memory_budget_mbytes)
	{
		init_logging();
		auto options = sbs::RenderOptions();
		options.mMemoryBudget = memory_budget_mbytes * 1024 * 1024;
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	explicit Context(const sbs::RenderOptions& options)
	{
		init_logging();
		renderer = std::make_unique<sbs::Renderer>(options);
	}

	~Context()
	{
		logger->info("Shutdown SBS context");
		logger->flush();
	}

	auto set_renderer_options(const sbs::RenderOptions& options) const -> void
	{
		if (!renderer) return;
		renderer->setOptions(options);
		renderer->run();
	}

	auto load_package(const std::string& filename, bool instantiate = true) -> result<Package*>;
	auto add_log_callback(LogCallback callback) {
		logger->info("Adding log callback");
		logger->sinks().push_back(std::make_shared<LogSinkMt>(callback));
	}

	template <typename... Args>
	inline void log_info(spdlog::format_string_t<Args...> fmt, Args&& ... args) { logger->info(fmt, args...); }
	template <typename... Args>
	inline void log_debug(spdlog::format_string_t<Args...> fmt, Args&& ... args) { logger->debug(fmt, args...); }
	template <typename... Args>
	inline void log_trace(spdlog::format_string_t<Args...> fmt, Args&& ... args) { logger->trace(fmt, args...); }
	template <typename... Args>
	inline void log_warn(spdlog::format_string_t<Args...> fmt, Args&& ... args) { logger->warn(fmt, args...); }
	template <typename... Args>
	inline void log_error(spdlog::format_string_t<Args...> fmt, Args&& ... args) { logger->error(fmt, args...); }
};

}

