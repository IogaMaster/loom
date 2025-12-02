#include "sol/state.hpp"
#include <loom/utils/lua_module.hpp>

#define DEBUGGER_LUA_IMPLEMENTATION
#include "debugger_lua.h"

extern "C" void register_loom_module(sol::state &lua) {
  dbg_setup(lua.lua_state(), "loom.debugger", "", nullptr, nullptr);
  auto debugger = loom::utils::create_lua_module(lua, "debugger");
  auto module = lua.require("loom.debugger", nullptr);
  lua["loom"]["debugger"] = module;
}
