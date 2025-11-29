#pragma once

#include "sol/state.hpp"
namespace loom::utils {
auto create_lua_module(sol::state &lua, std::string mod_name) -> sol::table {
  sol::table loom = lua["loom"];
  if (!loom.valid()) {
    loom = lua.create_named_table("loom");
  }
  sol::table mod = loom[mod_name];
  if (!mod.valid()) {
    mod = loom.create_named(mod_name);
  }
  return mod;
}
} // namespace loom::utils
