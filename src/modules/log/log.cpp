#include "sol/state.hpp"
#include <spdlog/spdlog.h>

#include <loom/utils/lua_module.hpp>

extern "C" void register_loom_module(sol::state &lua) {
  auto log = loom::utils::create_lua_module(lua, "log");

  log.set_function("trace", [](const std::string &msg) -> void {
    spdlog::trace("[Lua] {}", msg);
  });

  log.set_function("debug", [](const std::string &msg) -> void {
    spdlog::debug("[Lua] {}", msg);
  });

  log.set_function("info", [](const std::string &msg) -> void {
    spdlog::info("[Lua] {}", msg);
  });

  log.set_function("warn", [](const std::string &msg) -> void {
    spdlog::warn("[Lua] {}", msg);
  });

  log.set_function("error", [](const std::string &msg) -> void {
    spdlog::error("[Lua] {}", msg);
  });

  log.set_function("critical", [](const std::string &msg) {
    spdlog::critical("[Lua] {}", msg);
  });
}
