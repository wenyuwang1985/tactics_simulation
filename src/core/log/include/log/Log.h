#pragma once

#include <spdlog/spdlog.h>

namespace logging {

bool init(const char* name,
          const char* logfile,
          spdlog::level::level_enum level);

spdlog::logger* get();

} // namespace logging

// ===== 业务宏（推荐方式）=====

#define LOG_DEBUG(...)  SPDLOG_LOGGER_DEBUG(logging::get(), __VA_ARGS__)
#define LOG_INFO(...)   SPDLOG_LOGGER_INFO(logging::get(),  __VA_ARGS__)
#define LOG_WARN(...)   SPDLOG_LOGGER_WARN(logging::get(),  __VA_ARGS__)
#define LOG_ERROR(...)  SPDLOG_LOGGER_ERROR(logging::get(), __VA_ARGS__)
