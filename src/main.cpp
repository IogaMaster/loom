#include <loom/utils/load_modules.hpp>
#include <sol/sol.hpp>
#include <spdlog/cfg/env.h>

namespace {

struct State {
  sol::state lua;
  sol::environment sandbox;
};

auto init() -> State {
  State state;

  spdlog::cfg::load_env_levels();

  state.lua.open_libraries(sol::lib::base, sol::lib::package,
                           sol::lib::coroutine, sol::lib::string, sol::lib::os,
                           sol::lib::math, sol::lib::table, sol::lib::io,
                           sol::lib::debug, sol::lib::bit32, sol::lib::utf8);
  state.sandbox =
      sol::environment(state.lua, sol::create); // Create blank sandbox
  loom::utils::load_and_run_register(state.lua, state.sandbox);

  return state;
}

} // namespace

auto main() -> int {
  auto state = init();

  state.lua.script(R"(
    loom.log.trace("trace")
    loom.log.debug("debug")
    loom.log.info("info")
    loom.log.warn("warn")
    loom.log.error("Gaffor")
    loom.log.critical("Critical")
  )");

  return 0;
};
