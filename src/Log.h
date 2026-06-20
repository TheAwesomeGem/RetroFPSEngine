#pragma once
#include <spdlog/spdlog.h>
#include <string_view>

namespace Log
{
void init();

template <typename... Args> void debug(std::string_view category, spdlog::format_string_t<Args...> fmt, Args &&...fwd_args)
{
    std::string msg = fmt::format(fmt, std::forward<Args>(fwd_args)...);
    spdlog::debug("[{}] {}", category, msg);
}

template <typename... Args> void info(std::string_view category, spdlog::format_string_t<Args...> fmt, Args &&...fwd_args)
{
    std::string msg = fmt::format(fmt, std::forward<Args>(fwd_args)...);
    spdlog::info("[{}] {}", category, msg);
}

template <typename... Args> void warn(std::string_view category, spdlog::format_string_t<Args...> fmt, Args &&...fwd_args)
{
    std::string msg = fmt::format(fmt, std::forward<Args>(fwd_args)...);
    spdlog::warn("[{}] {}", category, msg);
}

template <typename... Args> void error(std::string_view category, spdlog::format_string_t<Args...> fmt, Args &&...fwd_args)
{
    std::string msg = fmt::format(fmt, std::forward<Args>(fwd_args)...);
    spdlog::error("[{}] {}", category, msg);
}

template <typename... Args> void fatal(std::string_view category, spdlog::format_string_t<Args...> fmt, Args &&...fwd_args)
{
    std::string msg = fmt::format(fmt, std::forward<Args>(fwd_args)...);
    spdlog::critical("[{}] {}", category, msg);
    assert(false);
}
}; // namespace Log
