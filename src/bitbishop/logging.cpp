#include <bitbishop/logging.hpp>

void Log::init() {
  SPDLOG_TRACE("Trace logging is compiled and active.");
  SPDLOG_DEBUG("Debug logging is compiled and active.");
  SPDLOG_INFO("Info logging is compiled and active.");
  SPDLOG_WARN("Warn logging is compiled and active.");
  SPDLOG_ERROR("Error logging is compiled and active.");
  SPDLOG_CRITICAL("Critical logging is compiled and active.");

  // Compile-time and run-time gates are at the same level for now.
  const auto log_level = static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL);
  spdlog::set_level(log_level);
  spdlog::info("Log level initialized to: {}", spdlog::level::to_string_view(log_level));
}
