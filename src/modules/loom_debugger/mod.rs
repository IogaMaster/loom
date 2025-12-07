use log::*;
use mlua::prelude::*;

pub fn init_loom_debugger(lua: &Lua) -> LuaResult<LuaTable> {
    let exports = lua.load(include_str!("./debugger.lua")).eval()?;
    Ok(exports)
}
