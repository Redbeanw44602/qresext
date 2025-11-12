// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

namespace qresext::__logging_impl {

constexpr std::string_view gray   = "\033[90m";
constexpr std::string_view yellow = "\033[33m";
constexpr std::string_view red    = "\033[31m";
constexpr std::string_view reset  = "\033[0m";

enum LogLevel { INFO, WARN, ERROR, DEBUG };

template <class... Args>
constexpr void log(LogLevel level, std::string_view fmt, Args&&... args) {
    // clang-format off
    std::string_view level_prefix =
        (level == WARN  ? "warning: " :
         level == ERROR ? "error: "   :
                          "");

    std::string_view level_color =
        (level == WARN  ? yellow :
         level == ERROR ? red    :
         level == DEBUG ? gray   :
                          "");
    // clang-format on

    if (level_color == "") {
        std::println(
            "{}{}",
            level_prefix,
            std::vformat(fmt, std::make_format_args(args...))
        );
    } else {
        std::println(
            "{}{}{}{}",
            level_color,
            level_prefix,
            std::vformat(fmt, std::make_format_args(args...)),
            reset
        );
    }
}

} // namespace qresext::__logging_impl

#ifndef QRESEXT_NO_LOGGING

#define QRESEXT_LOGGING ::qresext::__logging_impl::

#define logi(...) QRESEXT_LOGGING log(QRESEXT_LOGGING INFO, __VA_ARGS__)
#define logw(...) QRESEXT_LOGGING log(QRESEXT_LOGGING WARN, __VA_ARGS__)
#define loge(...) QRESEXT_LOGGING log(QRESEXT_LOGGING ERROR, __VA_ARGS__)
#define logd(...) QRESEXT_LOGGING log(QRESEXT_LOGGING DEBUG, __VA_ARGS__)

#else

#define logi(...)
#define logw(...)
#define loge(...)
#define logd(...)

#endif
