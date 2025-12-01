#include "sol/state.hpp"
#include <loom/utils/lua_module.hpp>

namespace {
const std::string embeddedData = {
#embed "debugger.lua" // NOLINT
};
} // namespace

extern "C" void register_loom_module(sol::state &lua) {
  lua.script(embeddedData);
}
